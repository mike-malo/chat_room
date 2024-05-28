# win10打包教学

## 需要有node环境

使用以下命令安装electron和electron-packager

`npm install electron -g`

`npm install electron-packager -g`

<!--其中-g参数是global全局安装的意思, -d是在当前项目目录安装的意思-->

## 使用electron打包网页需要以下步骤

### 运行文件

新建文件package.json

```json
{
	"name": "app",
	"main": "main.js"
}
```



新建文件main.js

```js
const { app, BrowserWindow } = require('electron')
const path = require('path')
const url = require('url')

// 保持一个对于 window 对象的全局引用，不然，当 JavaScript 被 GC，
// window 会被自动地关闭
var mainWindow = null;

// 当所有窗口被关闭了，退出。
app.on('window-all-closed', function () {
    // 在 OS X 上，通常用户在明确地按下 Cmd + Q 之前
    // 应用会保持活动状态
    if (process.platform != 'darwin') {
        app.quit();
    }
});

// 当 Electron 完成了初始化并且准备创建浏览器窗口的时候
// 这个方法就被调用
app.on('ready', function () {
    // 创建浏览器窗口。
    mainWindow = new BrowserWindow({ width: 800, height: 600 });

    // 加载应用的 index
    mainWindow.loadURL("http://www.baidu.com")

    // 打开开发工具
    // mainWindow.openDevTools();

    // 当 window 被关闭，这个事件会被发出
    mainWindow.on('closed', function () {
        // 取消引用 window 对象，如果你的应用支持多窗口的话，
        // 通常会把多个 window 对象存放在一个数组里面，
        // 但这次不是。
        mainWindow = null;
    });
});

```

使用命令

·electron-packager . app --win --out ./appBox --arch=x64 --version1.0.0 --icon=ico.ico --overwrite --ignore=node_modules·

这句命令的意思是`electron-packager . `exe文件名称` --win --out ./`文件夹名称` --arch=`64位` --version`版本号` --icon=`打包后文件的图标` --`每次调用覆盖文件` --ignore=`不打包的内容



# win7打包

electron新版本(22及以上)打包的应用在win7平台无法正常使用

## 解决方案

安装旧版的electron和electron-packager

`electron v11.5.0`

`electron-packager v12.1.0`

使用刚才的命令打包出来的应用可以在win7平台上使用



