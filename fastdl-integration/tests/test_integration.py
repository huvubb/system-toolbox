import importlib.util
import os
import subprocess
import sys
import threading
from contextlib import contextmanager
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))
spec = importlib.util.spec_from_file_location("toolbox_fastdl", ROOT / "dl.py")
adapter = importlib.util.module_from_spec(spec)
spec.loader.exec_module(adapter)
PAYLOAD = bytes(range(256)) * 2048


@contextmanager
def server(ranges):
    class Handler(BaseHTTPRequestHandler):
        def do_GET(self):
            start, end = 0, len(PAYLOAD) - 1
            partial = ranges and bool(self.headers.get("Range"))
            if partial:
                interval = self.headers["Range"].removeprefix("bytes=").split("-")
                start = int(interval[0])
                end = min(int(interval[1]) if interval[1] else end, end)
            self.send_response(206 if partial else 200)
            self.send_header("Content-Length", str(end - start + 1))
            self.send_header("Content-Disposition", 'attachment; filename="server-name.bin"')
            self.send_header("ETag", '"fixture"')
            if partial:
                self.send_header("Content-Range", f"bytes {start}-{end}/{len(PAYLOAD)}")
            self.end_headers()
            self.wfile.write(PAYLOAD[start:end + 1])

        def log_message(self, *args):
            pass

    http = ThreadingHTTPServer(("127.0.0.1", 0), Handler)
    thread = threading.Thread(target=http.serve_forever, daemon=True)
    thread.start()
    try:
        yield f"http://127.0.0.1:{http.server_port}/file?x=a&y=b"
    finally:
        http.shutdown()
        http.server_close()
        thread.join()


@pytest.mark.parametrize("ranges", [True, False])
def test_exact_output_from_real_fastdl(tmp_path, ranges):
    output = tmp_path / "中文 O'Brien & target.zip"
    output.write_bytes(b"old data")
    command = [os.environ["FASTDL_EXECUTABLE"]] if os.environ.get("FASTDL_EXECUTABLE") else [sys.executable, str(ROOT / "dl.py")]
    with server(ranges) as url:
        result = subprocess.run(command + ["toolbox-download", url, str(output), "--threads", "2"],
                                capture_output=True, timeout=40)
        assert result.returncode == 0, result.stderr
    assert output.read_bytes() == PAYLOAD
    if os.environ.get("TOOLBOX_TEST_EXECUTABLE"):
        toolbox_output = tmp_path / "工具箱结果.zip"
        with server(ranges) as url:
            result = subprocess.run([os.environ["TOOLBOX_TEST_EXECUTABLE"]], capture_output=True, timeout=40,
                                    env={**os.environ, "TOOLBOX_TEST_DOWNLOAD_URL": url,
                                         "TOOLBOX_TEST_DOWNLOAD_OUTPUT": str(toolbox_output)})
            assert result.returncode == 0, result.stderr
        assert toolbox_output.read_bytes() == PAYLOAD


def test_failure_preserves_output_and_resume(tmp_path, monkeypatch):
    output = tmp_path / "target.bin"
    output.write_bytes(b"old data")

    def partial(url, dest_dir, **kwargs):
        (Path(dest_dir) / "file.fastdl").write_bytes(b"partial")
        return 1

    monkeypatch.setattr(adapter, "download", partial)
    assert adapter.download_to_file("https://example.test/file", output) == 1
    assert output.read_bytes() == b"old data"
    assert list(tmp_path.glob(".toolbox-fastdl-*/file.fastdl"))
