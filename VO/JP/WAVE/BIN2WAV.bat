for %%A IN (*.bin) do (
"ffmpeg.exe" -f s8 -ar 11025 -ac 1 -i "%%~A" "%%~A.wav"
echo  Converting RAW to WAVE...
)
PAUSE