compile with <br>
gcc -Wno-format -o hash HashFile.c jtRecord.c -Wno-deprecated-declarations -Wno-format-security -lm `pkg-config --cflags --libs gtk+-3.0` -export-dynamic  <br>
run with  <br>
sudo ./hash  <br>
