#  星火商店

#### 介绍
星火商店商店，由深度社区爱好者维护
web页面部分正在开发当中，详情请见[web仓库](https://gitee.com/deepin-community-store/DCSAPP_WEB)

#### 说明

当前服务器线路列表（项目中包含）：

```
http://dcstore.shenmo.tech/
http://store.jerrywang.top/
```

#### 参数

参数只有一个Url，该url应当遵循这种格式：`spk://<任意合法字符>/web分类/包名`

例如:

[spk://abcdefg/network/firefox-zh](spk://abcdefg/network/firefox-zh)

[spk://higklmn/network/firefox-zh](spk://higklmn/network/firefox-zh)

可选的web分类：

| 分类名称 | web分类       |
| -------- | ------------- |
| 网络应用 | network       |
| 社交沟通 | relations     |
| 音乐欣赏 | MusicAndSound |
| 视频播放 | Videos        |
| 图形图像 | Photos        |
| 游戏娱乐 | Games         |
| 办公学习 | Office        |
| 阅读翻译 | Reading       |
| 编程开发 | Programming   |
| 系统工具 | Tools         |
| 主题美化 | Themes        |
| 其他应用 | Others        |

 #### 目录结构

安装软件过程中产生的包，图标，截图等被储存到`/tmp/deepin-community-store/`中。

配置文件被储存到`~/.config/deepin-community-store/`中。

线路文件：新版的线路文件被放置于源服务器中，可随时刷新更新源列表

#### 参与贡献

1.  Fork 本仓库
