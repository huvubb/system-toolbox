from types import SimpleNamespace

from music_unlock.transcode import transcode


def test_hardware_attempt_falls_back(monkeypatch, tmp_path):
    calls = []
    destination = tmp_path / "out.mp3"
    monkeypatch.setattr("music_unlock.transcode.resolve_ffmpeg", lambda _: "ffmpeg")

    def run(args, **kwargs):
        calls.append(args)
        if "-hwaccel" in args:
            return SimpleNamespace(returncode=1, stderr="hardware unavailable")
        destination.write_bytes(b"mp3")
        return SimpleNamespace(returncode=0, stderr="")

    monkeypatch.setattr("music_unlock.transcode.subprocess.run", run)
    transcode(tmp_path / "in.wav", destination, "mp3")
    assert len(calls) == 2
    assert "-hwaccel" in calls[0] and "-hwaccel" not in calls[1]
