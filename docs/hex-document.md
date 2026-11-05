# 十六进制文档 / Hex documents

`WingHexDocument` 管理文档访问句柄，封装打开、关闭、释放、信息查询和激活。局部对象析构仅释放句柄，编辑器保持打开。读写、光标、选择、查找、标记、标注和显示控制复用插件原有接口，须在 `use()` 作用域内调用。
`WingHexDocument` owns a document access handle and wraps opening, closing, release, information queries and activation. Destruction releases access while leaving the editor open. Editing, cursor, selection, search, bookmarks, metadata and display controls reuse the existing plugin API and require a live `use()` scope.

## 打开与访问 / Opening and access

```cpp
#include <WingPlugin/winghexdocument.h>

void inspectFile(WingHex::IWingPlugin *plugin, const QString &path) {
    WingHex::WingHexDocument document(plugin);
    if (document.openFile(path) != WingHex::ErrFile::Success) {
        return;
    }
    const auto size = document.documentBytes();
    Q_UNUSED(size);
    Q_UNUSED(document.activate());
} // Releases access; the tab remains open. / 释放访问，标签页保持打开。
```

空对象可选择三种打开方式，各自返回 `int` 错误码（取值见 `ErrFile`）。重复打开复用编辑器，每次取得独立访问句柄；超过每插件 256 个访问句柄时返回 `TooManyOpenedFile`。打开已有有效对象返回 `AlreadyOpened`，不替换当前文档。
An empty object offers three opening methods returning integer error codes from `ErrFile`. Repeated opens reuse the editor and acquire independent access handles. The per-plugin limit is 256 handles (`TooManyOpenedFile`). Opening an already valid object returns `AlreadyOpened` without replacing its document.

```cpp
WingHex::WingHexDocument file(plugin);
auto result = file.openFile(filePath);

WingHex::WingHexDocument device(plugin);
result = device.openExt(driverId, devicePath);

WingHex::WingHexDocument workspace(plugin);
result = workspace.openWorkSpace(workspacePath);
```

`openFile(QString)` 按原始文件打开，即使文件名以 `.wingpro` 结尾。`openWorkSpace()` 加载工作区及其普通文件或插件设备、标记和标注。若源文档已按普通文件或另一工作区打开，则返回 `AlreadyOpened`，不会静默覆盖其状态。工作区指纹检查沿用宿主行为。
`openFile(QString)` opens raw bytes even for a `.wingpro` filename. `openWorkSpace()` loads the workspace, its ordinary file or plugin device, bookmarks and metadata. If the source is already open without this workspace or with another workspace, it returns `AlreadyOpened` without replacing that state. Fingerprint checks follow the host's existing behavior.

本地相对路径相对于调用时工作目录解析。设备路径是驱动接收的原始字符串，可包含前导 `/`、中文和 URL 保留字符，无需预先编码。
Relative local paths resolve against the working directory at call time. A device path is the original string received by the driver; leading slashes, Unicode and URL-reserved characters need no manual encoding.

## 信息与编辑 / Information and editing

可直接通过 `fileUrl()`、`workspaceUrl()`、`documentType()`、`documentBytes()` 和 `isValid()` 读取文档属性。属性每次查询当前状态，不长期缓存；另存为、工作区变化、编辑和外部关闭后不会沿用旧快照。需要同时读取多个字段时，调用一次 `documentInfo()` 并在本地保留快照。信息属性查询允许后台读取。
Read document properties through `fileUrl()`, `workspaceUrl()`, `documentType()`, `documentBytes()`, `isValid()`. Properties query current state without persistent caching, so save-as, workspace changes, edits and external closure cannot leave an old snapshot in use. For several fields together, call `documentInfo()` once and keep the snapshot locally. Information properties allow background reads.

`documentInfo()` 返回 `DocumentInfo` 快照：`isValid`、`fileNameUrl`、`workSpaceName`、`documentType`、`isSaved`、`isReadOnly` 和 `documentBytes`。`workSpaceName` 为实际工作区文件路径；空值表示没有工作区。`activate()` 激活编辑器，读取或修改本身不改变激活标签页。
`documentInfo()` returns a `DocumentInfo` snapshot: `isValid`, `fileNameUrl`, `workSpaceName`, `documentType`, `isSaved`, `isReadOnly` and `documentBytes`. `workSpaceName` is the path of the actual workspace file, or empty for a non-workspace document. `activate()` activates the editor; reading and editing do not change the active tab.


## 关闭与所有权 / Closing and ownership

- `close()` 真正关闭编辑器。未保存时返回 `UnSaved`，对象继续有效，不显示宿主保存确认框。/ `close()` closes the editor. Unsaved edits return `UnSaved`; the object remains valid and no host save prompt is shown.
- `close(true)` 沿用宿主的强制关闭行为；失败时保留句柄供重试。/ `close(true)` follows the host's force-close behavior and retains the handle on failure.
- 关闭成功后对象失效；其他对象和插件中引用相同编辑器的句柄也失效。用户关闭标签页具有相同效果。/ Successful closure invalidates this object and all handles referencing the same editor, including other plugins. User closure has the same effect.
- 析构仅释放自身访问句柄，不关闭文档。插件销毁自动清理遗留访问，插件销毁后遗留对象安全失效。/ Destruction releases only its access handle. Plugin destruction cleans up remaining access and safely invalidates surviving objects.
- 对象不可复制、允许移动；移动赋值释放旧句柄而不关闭旧标签页。失效对象可再次打开文档。/ Objects are non-copyable and movable. Move assignment releases the old handle without closing its tab. An invalid object can open another document.

原生 C++ 没有“脚本执行结束”事件：局部对象离开作用域通过 RAII 清理，插件卸载提供兜底清理。Luau 现有的执行上下文清理和接口语义本次保持不变，后续再同步新 API。
Native C++ has no script-completion event: local objects release through RAII, with plugin unloading as a fallback. Existing Luau execution-context cleanup and API semantics remain unchanged in this update; migration follows separately.

## 原有接口兼容 / Existing API compatibility

保留原有插件 `public slots` 的名称、参数顺序、参数类型和返回类型，以及原有 SDK 的调用签名。`WingHexDocument` 使用独立的文档调用对象转发到内部实现，不改变全局插件调用表中的原有条目，也不切换插件当前文档。新增的文档扩展入口独立声明。
Existing plugin public-slot names, parameter order, parameter types, return types and SDK call signatures are preserved. `WingHexDocument` forwards through a separate document receiver to internal implementations, without changing existing global plugin call-table entries or switching the plugin's current document. New document extension entries are declared separately.

## 插件底层入口与迁移 / Low-level plugin entry and migration

SDK 版本为 22，原生插件需要重新编译。`WingHexDocument` 不公开 `openFile(QUrl)`；插件本身保留通用入口和 URL 构造函数：
SDK version 22 requires rebuilding native plugins. `WingHexDocument` does not expose `openFile(QUrl)`; the plugin itself retains the generic entry and URL helpers:

```cpp
auto local = WingHex::fileUrl(filePath);                  // file:
auto device = WingHex::deviceUrl(driverId, devicePath);   // wdrv:
auto workspace = WingHex::workspaceUrl(workspacePath);    // wingpro:
auto handle = plugin->openFile(workspace);
if (handle >= 0) {
    const auto info = plugin->documentInfo(handle);
    Q_UNUSED(size);
    Q_UNUSED(plugin->closeFile(handle)); // Really closes the editor.
}
```

URL 构造仅发生在 SDK 内，不调用宿主。通用入口拒绝未知协议、query、fragment、认证信息和端口；路径中的这些字符应由构造函数编码。工作区使用 `wingpro:`，不通过扩展名猜测。
URL construction is local to the SDK. The generic entry rejects unknown schemes, queries, fragments, credentials and ports; path characters are encoded by the helpers. Workspaces use `wingpro:` rather than filename guessing.

旧 `openFile(QUrl)` 仅绑定已打开文档，现在会真正打开；旧 `closeFile(handle)` 仅释放句柄，现在真正关闭。仅需临时访问时改用局部 `WingHexDocument`，让其析构释放，不能再用 `closeFile()` 模拟释放。旧 `openCurrent()`、`closeCurrent()` 以及当前目标读写接口暂保留原有迁移语义，新的文档对象不依赖它们。
Previously, `openFile(QUrl)` only bound an existing editor; it now opens it. Previously, `closeFile(handle)` released access; it now closes the editor. For temporary access use a local `WingHexDocument` and let destruction release it; do not use `closeFile()` for release. Legacy `openCurrent()`, `closeCurrent()` and current-target editing calls retain their migration behavior; the new document object does not depend on them.

`closeHandle()` 显式释放当前对象的访问句柄，返回 `int` 错误码；不会关闭编辑器，成功后对象失效并可重新打开。析构仍自动释放句柄。
`closeHandle()` explicitly releases this object's access handle and returns an integer error code. The editor stays open; success invalidates the object and allows it to be opened again. Destruction still releases access automatically.

## 操作作用域 / Operation scope

```cpp
if (auto usage = document.use(); usage) {
    const auto bytes = document.readBytes(0, 16);
    const auto found = document.findNext(0, QByteArrayLiteral("signature"));
    Q_UNUSED(bytes);
    Q_UNUSED(found);
} // Further document operations fail. / 后续文档操作失败。
```

`use()` 首次进入通过插件接口校验 GUI 线程并调用一次 `switchDocument(handle)`，不会激活编辑器。单次作用域内的操作直接使用现有调用表，不逐个切换文档。退出后不恢复插件之前的目标；只有 `WingHexDocument` 的操作受作用域保护，直接调用插件原有 API 的行为不变。
The first entry to `use()` relies on the plugin interface to enforce GUI affinity and calls `switchDocument(handle)` once without activating the editor. Operations reuse the existing call table without switching individually. Leaving the scope does not restore the previous plugin target. The guard applies to `WingHexDocument` operations; direct calls to the existing plugin API retain their behavior.

同一对象允许嵌套作用域，不重复切换。同一插件的不同文档不能重叠使用，第二个 `use()` 返回无效守卫；不同插件独立。关闭、释放或销毁对象使其守卫失效；移动对象保留作用域。守卫可移动，不可复制，并可安全地晚于文档对象析构。
Nested scopes on the same object do not switch again. Overlapping scopes on different documents owned by one plugin are rejected; different plugins are independent. Closing, releasing or destroying a document invalidates its guards. Moving a document preserves its scope. Guards are movable, noncopyable and may safely outlive the document object.

信息属性、打开、关闭和激活无需作用域。`read*` 和查找可以在 GUI 线程已建立的作用域内并发调用；并发读取期间保持文档、插件及守卫有效，不同时写入、关闭或切换目标。作用域内也不应直接使用插件 API 切换目标，否则后续操作将使用被切换后的插件目标。
Information properties, opening, closing and activation do not require a scope. Reads and searches may run concurrently under a scope entered on the GUI thread. Keep the document, plugin and guard alive, and do not concurrently write, close or switch targets. Do not directly switch the plugin target within the scope: subsequent operations would use that changed target.
