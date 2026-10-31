# Luau 定义文件 / Luau definition files

在 `onRegisterScriptObj` 中使用同一个 `ScopeBuilder` 注册运行时接口和定义文件，无需改变回调签名：

Register runtime objects and definition files through the same `ScopeBuilder` in `onRegisterScriptObj`; the callback signature is unchanged:

定义文件必须在注册阶段提交；注册结束后追加文件会抛出 `RegistrationFailed`。

Submit definition files during registration; adding files after registration finishes raises `RegistrationFailed`.

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

对象树展开插件节点时复用已解析的定义，显示函数签名、独立重载及嵌套成员。类构造器返回的外部类型用于解析实例方法。未在定义中声明的运行时成员仍然显示；定义不会改变运行时注册内容。

Expanding a plugin in the object tree reuses the parsed declarations for signatures, separate overload rows and nested members. External types returned by class constructors supply instance method declarations. Runtime members without declarations remain visible; definitions do not change runtime registration.
