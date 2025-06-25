# AGENTS.md

本仓库是一个将 wide_integer 从 clickhouse 中拆分出来的独立的 header-only 实现

## 项目结构
- `include/`：源码
- `tests/`: 单元测试
- `bench/`：性能测试

## 代码风格

每次编辑代码后使用 clang-format 进行格式化，配置文件为根目录下的 .clang-format，只针对
c++ 源码文件进行格式化

## Pull Request 规范

1. 创建分支时使用英文名
