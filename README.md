# RTPDemo

# 使用
1.安装IDE（QtCreator），同时需要Qt SDK，链接：https://download.qt.io/official_releases/qt/<br>
  编译的时候,Windows系统推荐Mingw 64bit,Linux系统使用GCC 64bit,不要使用32bit<br>
2.打开项目，需要在工程里面设置好lib的路径，在.pro文件里，找到`LIBS += -L$$PWD/ -lrtplivelib`<br>
  将$$PWD修改为自己lib路径即可,lib工程地址:https://github.com/ZhongHuaRong/rtplivelib<br>
3.执行qmake，然后点击run运行<br>

