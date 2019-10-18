compile with <br>
gcc -Wno-format -o hash HashFile.c jtRecord.c -Wno-deprecated-declarations -Wno-format-security -lm `pkg-config --cflags --libs gtk+-3.0` -export-dynamic  <br>
run with  <br>
sudo ./hash  <br>
// yum install gtk3 gtk3-devel gtk3-devel-docs <br>
// yum install gnome-devel gnome-devel-docs <br><br>

// sudo apt-get install libgtk2.0*     用的是3 apt不清楚 <br>

界面编辑工具 Glade<br>
