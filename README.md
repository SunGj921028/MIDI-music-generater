# MIDI-音樂產生器

### Environment
1. Linux or WSL

### How to use
1. 用gcc編譯此檔案
2. 準備一個txt樂譜檔
3. 執行執行檔

#### 檔案格式
1. The input file consists of numbers and symbols, where numbers represent musical notes and symbols represent duration and pitch. Specifically, the numbers ”1” to ”7” represent Middle Do to Middle Si, ”0” represents a rest, and ”-”, ”.”, ”;”, ”'”, and ”,” represent changes in duration and pitch.
2. Specifically, the symbols ”-”, ”.”, and ”;” denote double duration, one and a half duration, and half duration respectively. The symbols ”'”, and ”,” denote an octave up and an octave down respectively. For instance, ”-” represents a half note, and ”--” represents a whole note; ”.” represents a dotted note; ”;” represents an eighth note, and ”;;” represents a sixteenth note; ”'” represents a note one octave higher, such as ”1'”, and ”,” represents a note one octave lower, such as ”1,”.
3. The duration symbol will come after the octave symbol, e.g., ”1''--”.
4. Please ignore all white space characters in the input file, such as ' ' and '\n'.
5. For your simplicity, the output will be set to 120 BPM, which is the default MIDI tempo. There is no need for you to make any changes.
6. There may be many instruments, and each instrument will be marked as ”[INSTRUMENT]” at the beginning of its respective part.
7. The input of the program is a ”.txt” file of numerical notation, and the output is a ”.mid” file.
8. I guarantee that there are no wrong inputs, so you don’t need to worry about that.
