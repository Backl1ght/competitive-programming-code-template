# competitive-programming-code-template

## 使用方法

1. 直接使用编译好的pdf文件
2. 使用`src`文件夹下的源代码文件

若想对模板进行修改，直接修改`head`，`tail`，`main.py`或者源代码文件，后运行`main.py`生成`template.tex`，再编译生成`template.pdf`。其中：

- `head`：主要是tex的一些配置，如格式，目录等；
- `tail`：补充head。
- `main.py`：根据源代码生成`template.tex`的逻辑。`src`文件夹下，文件夹名为一级标题，文件夹下的源代码为二级标题。没有额外的配置文件，默认将所有源代码都包含进来。
- `template.tex`：生成的tex源码。
- `template.pdf`：生成的pdf文件。

## 特性

1. 使用了`mathescape`属性，源代码注释中的数学公式会被编译并渲染。

## 简介

- 对于一些复用性较高的算法或者数据结构，需要时再重新实现一次是费时的，如果有一份设计得不错得代码模板，可能会花费更少的时间。
- 在实现实现某个算法时，如果有类似算法的代码作为参考，可能效率会比较高。
- 复习所学内容时，如果将已学内容的实现记录下来的话，可能会有更好的效果。

基于以上目的，我自己会实现一些常见算法的代码模板。经过一段时间的完善，自认为写得还行，可能会有参考意义，所以也将这个代码模板开源了。

模板既包括源代码文件夹，也包括方便打印的pdf版本。

这个模板中更多的是源码和说明性的注释，没有对于原理的解释。如果想要了解原理，百度，Bing，Google，OI Wiki，Luogu甚至B站上，可能会有你想要的东西。

## 参考

本项目参考了[ACM-ICPC-CodeTemplate-Latex](https://github.com/jasison27/ACM-ICPC-CodeTemplate-Latex)。
