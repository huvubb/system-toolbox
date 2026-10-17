"""fastdl entry point with an exact-output adapter for System Toolbox."""
import argparse
import hashlib
import ipaddress
import os
import sys
from pathlib import Path
from urllib.parse import urlsplit

from fastdl.cli import main as fastdl_main
from fastdl.config import load_config
from fastdl.direct import download


def download_to_file(url: str, destination: Path, threads: int = 0) -> int:
    import msvcrt

    destination = destination.resolve()
    destination.parent.mkdir(parents=True, exist_ok=True)
    identity = hashlib.sha256((url + "\0" + str(destination)).encode()).hexdigest()[:24]
    staging = destination.parent / (".toolbox-fastdl-" + identity)
    staging.mkdir(exist_ok=True)
    # Windows releases this lock if a queued download is terminated.
    with (staging / ".lock").open("a+b") as lock:
        lock.seek(0)
        if lock.read(1) == b"":
            lock.write(b"0")
            lock.flush()
        lock.seek(0)
        try:
            msvcrt.locking(lock.fileno(), msvcrt.LK_NBLCK, 1)
        except OSError:
            print("同一文件正在下载，请稍后重试。", file=sys.stderr)
            return 1
        try:
            host = urlsplit(url).hostname or ""
            config = load_config()
            threads = max(1, min(128, threads or config.threads))
            source_ip = "127.0.0.1" if host == "localhost" else config.iface_ip or None
            try:
                if ipaddress.ip_address(host).is_loopback:
                    source_ip = host
            except ValueError:
                pass
            code = download(url, dest_dir=str(staging), threads=threads, resume=True, source_ip=source_ip)
            if code:
                return code
            files = [p for p in staging.iterdir() if p.is_file() and p.name != ".lock"
                     and not p.name.endswith((".fastdl", ".part", ".dlmeta"))]
            if len(files) != 1:
                raise RuntimeError("下载结果不唯一或文件未完成，未替换目标文件")
            os.replace(files[0], destination)
            return 0
        finally:
            lock.seek(0)
            msvcrt.locking(lock.fileno(), msvcrt.LK_UNLCK, 1)


def main(argv=None):
    argv = sys.argv[1:] if argv is None else argv
    if not argv or argv[0] != "toolbox-download":
        return fastdl_main(argv)
    parser = argparse.ArgumentParser()
    parser.add_argument("command")
    parser.add_argument("url")
    parser.add_argument("destination", type=Path)
    parser.add_argument("--threads", type=int, choices=range(1, 129), default=0)
    args = parser.parse_args(argv)
    try:
        return download_to_file(args.url, args.destination, args.threads)
    except Exception as exc:
        print(f"下载失败: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    for stream in (sys.stdout, sys.stderr):
        if hasattr(stream, "reconfigure"):
            stream.reconfigure(encoding="utf-8", errors="replace")
    raise SystemExit(main())
