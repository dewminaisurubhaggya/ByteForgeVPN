# Feature status — Byte Forge VPN

Status values: `NOT_STARTED`, `IN_PROGRESS`, `IMPLEMENTED`, `TESTED`, `BLOCKED`.

A feature is never marked TESTED until it has been built and verified on the target environment.

| ID | Feature | Status | Source files | Testing / limitations |
|---|---|---|---|---|
| FND-001 | CMake C++20 project | IMPLEMENTED | CMakeLists.txt | Configured for Ninja/MSVC Release builds; verified in CI on `windows-2022` |
| FND-002 | Qt main window and sidebar | IMPLEMENTED | app/MainWindow.* | Fixed escaped newline in brand label; ready for Qt 6 MSVC kit |
| FND-003 | Dark theme and compact layout | IMPLEMENTED | app/MainWindow.cpp | Visual QA pending on Windows desktop |
| FND-004 | Dashboard page foundation | IMPLEMENTED | ui/Pages.* | Statistics intentionally show unavailable values (`—`) |
| FND-005 | Servers page foundation | IMPLEMENTED | ui/Pages.* | Wired to local SQLite profile import and deletion |
| FND-006 | Subscription page foundation | IMPLEMENTED | ui/Pages.* | UI only; HTTP fetch/parser not implemented |
| FND-007 | Tunneling page foundation | IMPLEMENTED | ui/Pages.* | UI only; no tunnel started |
| FND-008 | Routing page foundation | IMPLEMENTED | ui/Pages.* | UI only; no routes changed |
| FND-009 | Logs page foundation | IMPLEMENTED | ui/Pages.* | Fixed escaped newlines in startup diagnostics text |
| FND-010 | Settings page foundation | IMPLEMENTED | ui/Pages.* | Persistence and actions pending |
| FND-011 | SQLite schema initialization | IMPLEMENTED | core/Database.* | Creates `profiles`, `subscriptions`, `app_settings` tables in `AppLocalDataLocation` |
| FND-012 | README setup and cloud build instructions | IMPLEMENTED | README.md | Updated with GitHub Actions installer workflow instructions |
| FND-013 | Feature tracking document | IMPLEMENTED | FEATURE_STATUS.md | Updated for CI packaging & installer milestone |
| PRF-002 | Basic URI import and local profile listing/deletion | IMPLEMENTED | core/ProfileManager.*, ui/Pages.* | Supports VLESS, VMess (numeric & string ports), Trojan, Shadowsocks, SOCKS5, HTTP(S) URI storage; full protocol config generation pending |
| INS-001 | Inno Setup 6 Windows x64 installer & GitHub Actions CI | IMPLEMENTED | installer/ByteForgeVPN.iss, .github/workflows/build-windows-installer.yml | Automated build, `windeployqt` bundling, MSVC CRT bundling, Inno Setup packaging, and silent install/uninstall verification on `windows-2022` |
| ENG-001 | sing-box process integration | NOT_STARTED | — | Phase 3 |
| PRF-001 | Full profile editing, groups, favorites, QR and JSON import/export | NOT_STARTED | — | Remaining Phase 2 work |
| SUB-001 | Subscription fetch and parsing | NOT_STARTED | — | Phase 6 |
| TUN-001 | Wintun and TUN mode | NOT_STARTED | — | Phase 7 |
| SEC-001 | DPAPI credential storage | NOT_STARTED | — | Phase 10 |

## Build & test verification notes

- **Local Linux sandbox environment**: Qt 6 MSVC toolchain, `windeployqt.exe`, and Inno Setup 6 (`ISCC.exe`) are Windows-native binaries and cannot execute natively inside this Linux inspection container.
- **Cloud CI environment (`windows-2022`)**: The GitHub Actions workflow compiles `ByteForgeVPN.exe`, verifies DLL/plugin deployment (`Qt6Core.dll`, `Qt6Gui.dll`, `Qt6Widgets.dll`, `Qt6Sql.dll`, `platforms/qwindows.dll`, `sqldrivers/qsqlite.dll`), compiles `ByteForgeVPN_Setup.exe`, and runs an automated `/VERYSILENT` install/uninstall check on the Windows runner.
- **Manual interactive GUI check**: Launching the main window interactively with a desktop display session must be verified by running the downloaded `ByteForgeVPN_Setup.exe` on Windows 10 64-bit.
