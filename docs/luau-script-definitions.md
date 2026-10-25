# Luau 定义文件 / Luau definition files

在 `onRegisterScriptObj` 中使用同一个 `ScopeBuilder` 注册运行时接口和定义文件，无需改变回调签名：

Register runtime objects and definition files through the same `ScopeBuilder` in `onRegisterScriptObj`; the callback signature is unchanged:

```cpp
void MyPlugin::onRegisterScriptObj(WingScriptBind::ScopeBuilder scope) {
    scope.addFunction("twice", [](double value) { return value * 2; });
    scope.addDefinitionFile(QStringLiteral(":/MyPlugin/script/api.d.luau"));
}
```

资源文件 `api.d.luau` 使用 Luau 定义语法；全局命名空间必须与插件实际 PUID 一致，例如 PUID 为 `MyPlugin`：

Use Luau declaration syntax in the resource file, matching the plugin's actual PUID rather than assuming its C++ class name:

```luau
declare MyPlugin: {
    twice: (value: number) -> number,
}
```

也可以传入可读文件的绝对路径。相同文件重复注册只加载一次；文件按注册顺序在宿主公共定义之后加载，并由语言服务提供补全、类型检查及定义定位。文件不作为脚本执行；它描述接口，不创建运行时对象。相对路径、缺失文件及不可读文件会抛出 `ScriptException`，定义语法错误通过语言服务诊断报告。

Readable absolute filesystem paths are supported as well as Qt resource paths. Duplicate files load once, in registration order after host public definitions. The language server uses them for completion, type checking and definition navigation. Definitions are never executed and do not create runtime objects. Relative, missing or unreadable files raise `ScriptException`; declaration errors are reported through language-server diagnostics.
