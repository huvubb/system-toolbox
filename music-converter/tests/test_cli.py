import io
import os
import shutil
import subprocess
import sys
import wave
from pathlib import Path

import pytest

from builders import build_kgm, build_ncm, build_qmc_v1, load_kgm_pub_key
from music_unlock.formats.kgm import VPR_MAGIC, VPR_MASK


ROOT = Path(__file__).resolve().parents[1]


def command():
    helper = os.environ.get("MUSIC_CONVERTER_EXECUTABLE")
    return [helper] if helper else [sys.executable, str(ROOT / "unlocker.py")]


def test_converts_directory_to_mp3_without_overwriting(tmp_path):
    if not shutil.which("ffmpeg") or not shutil.which("ffprobe"):
        pytest.skip("ffmpeg and ffprobe are required")
    data = io.BytesIO()
    with wave.open(data, "wb") as audio:
        audio.setnchannels(1)
        audio.setsampwidth(2)
        audio.setframerate(8000)
        audio.writeframes(b"\0\0" * 800)
    inputs = tmp_path / "中文 输入 & music"
    output = tmp_path / "输出 文件"
    inputs.mkdir()
    output.mkdir()
    (inputs / "歌曲.ncm").write_bytes(build_ncm(data.getvalue(), b"0123456789abcdef", {"format": "wav"}))
    (inputs / "qq.bkcmp3").write_bytes(build_qmc_v1(data.getvalue()))
    (inputs / "k.kgm.flac").write_bytes(build_kgm(data.getvalue(), load_kgm_pub_key()))
    vpr_audio = bytes(b ^ VPR_MASK[i % len(VPR_MASK)] for i, b in enumerate(data.getvalue()))
    vpr = bytearray(build_kgm(vpr_audio, load_kgm_pub_key()))
    vpr[:16] = VPR_MAGIC
    (inputs / "v.vpr").write_bytes(vpr)
    (output / "歌曲.mp3").write_bytes(b"existing")
    result = subprocess.run(command() + ["--format", "mp3", "--no-embed-cover", "-o", str(output), str(inputs)],
                            capture_output=True, text=True, encoding="utf-8", errors="replace",
                            env={**os.environ, "PYTHONIOENCODING": "utf-8"})
    assert result.returncode == 0, result.stderr
    assert (output / "歌曲.mp3").read_bytes() == b"existing"
    for path in [output / "歌曲 (1).mp3", output / "qq.mp3", output / "k.kgm.mp3", output / "v.mp3"]:
        probe = subprocess.run(["ffprobe", "-v", "error", "-show_entries", "stream=codec_name",
                                "-of", "default=nw=1:nk=1", str(path)], capture_output=True, text=True)
        assert probe.returncode == 0 and probe.stdout.strip() == "mp3"

    toolbox_test = os.environ.get("TOOLBOX_TEST_EXECUTABLE")
    if toolbox_test:
        output_from_toolbox = tmp_path / "工具箱结果"
        result = subprocess.run([toolbox_test], capture_output=True, text=True, encoding="utf-8", errors="replace",
                                env={**os.environ, "PYTHONIOENCODING": "utf-8",
                                     "TOOLBOX_TEST_MUSIC_INPUT": str(inputs / "歌曲.ncm"),
                                     "TOOLBOX_TEST_MUSIC_OUTPUT": str(output_from_toolbox)})
        assert result.returncode == 0, result.stderr
        assert (output_from_toolbox / "歌曲.mp3").is_file()


def test_empty_directory_is_failure(tmp_path):
    result = subprocess.run(command() + [str(tmp_path)], capture_output=True, text=True, encoding="utf-8", errors="replace",
                            env={**os.environ, "PYTHONIOENCODING": "utf-8"})
    assert result.returncode != 0
