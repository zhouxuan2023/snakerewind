# 编码规范

## C++ 规则
- 使用现代 C++ (C++17+)
- 禁止裸指针（优先 smart pointer）
- 类命名：PascalCase
- 函数命名：camelCase

## Qt 规则
- 所有 QObject 必须包含 Q_OBJECT
- UI 与逻辑分离（QML/UI 不写业务逻辑）

## 文件结构
- .h 只放声明
- .cpp 放实现

## AI 生成代码要求
- 必须包含 namespace
- 必须有注释
- 不允许生成未使用代码