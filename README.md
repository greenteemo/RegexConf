# RegexConf

基于正则表达式的实时加载（读取）配置库



# 使用说明

1. 首先在src目录下执行 `sh make.sh` 编译生产动态库

2. demo目录下执行 `sh run.sh` 试用，可查看demo/main.cpp了解主要接口使用方法。



demo运行如下：

```shell
RegexConf Version: V0.0.0 (2021-04-13 16:11)

##### Test RegexConf #####
a:[11]
b:[22]

# 修改正则表达式匹配模式（配置项和对应值由等号分隔改为冒号分隔）
a:[01]
b:[02]

# 更换配置文件为 test.conf2
a:[03]
b:[05]

# 手动输入配置项获取对应配置值（可尝试修改配置文件，查看配置是否实时更新）
Get conf value of:a
03
Get conf value of:b
05
Get conf value of:b
04
Get conf value of:^C
```

注：b的值变化是因为手动修改了配置文件test.conf2中b的值。



# 其他

如有问题可提Issues或发邮件至greenteemo1013@gmail.com
