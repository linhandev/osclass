compile with
gcc -Wno-format -o hash HashFile.c jtRecord.c -Wno-deprecated-declarations -Wno-format-security -lm `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
run with
sudo ./hash
