# AGENTS.md

本项目是一个将 wide_integer 从 clickhouse 中拆分出来并改造的独立 header-only 库，旨在提供一个高性能、数据长度与定义严格一直并易于引入其它项目的宽整型实现

## 项目特点
1. 主要代码基于 c++17 标准实现，并提供兼容 c++11 标准的替代版本，性能相比 c++17 实现略有降低
2. `wide_integer` 的实际长度与定义严格一致，如 `wide_integer<256, true>` 类型的长度一定是 256bits，不会出现类似于 boost 中 int256 实际长度大于 256bits 的情况
3. 支持与 c++ 标准整型之间的算术运算和类型转换
4. 除格式化输出相关代码依赖 `fmt` 外，不依赖其他第三方库


## 项目结构
- `include/`: 源码
- `tests/`: 基于 gtest 实现的单元测试用例
- `bench/`: 基于 google benchmark 实现的性能测试用例

## 代码风格
每次编辑代码后使用 clang-format 进行格式化，配置文件为根目录下的 .clang-format，只针对c++ 源码文件进行格式化

## Pull Request 规范
创建开发分支时使用英文名
