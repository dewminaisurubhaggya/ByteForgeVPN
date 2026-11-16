# Byte Forge VPN — Windows Foundation & Cloud Installer

Modular Windows 10/11 (64-bit) VPN and tunneling client desktop foundation, configured for zero-setup cloud compilation and packaging into `ByteForgeVPN_Setup.exe` via GitHub Actions.

## Current scope & honesty notice

**Implemented:**
- Qt 6 Widgets desktop shell with compact dark theme
- Sidebar navigation across Dashboard, Servers, Subscriptions, Tunneling, Routing, Logs, Settings, and About pages
- Local SQLite database initialization (`profiles`, `subscriptions`, `app_settings`) under `QStandardPaths::AppLocalDataLocation`
- Local URI profile import, listing, and deletion for VLESS, VMess, Trojan, Shadowsocks, SOCKS5, and HTTP(S) links
- Automated GitHub Actions Windows x64 build (`windows-2022`), `windeployqt` + MSVC CRT dependency bundling, and Inno Setup 6 installer (`ByteForgeVPN_Setup.exe`)

**Not yet implemented (planned architecture):**
- `sing-box` engine process management and real VPN/proxy connections (the Connect button remains intentionally disabled)
- Live traffic counters, latency probing, System Proxy toggling, Wintun/TUN mode, subscription HTTP fetching, and DPAPI secret encryption

## How to build & download `ByteForgeVPN_Setup.exe` (No local Qt/Visual Studio needed)

1. Create a new repository on GitHub (or open your existing repository) and upload all files from this project folder so that `.github/workflows/build-windows-installer.yml` is at the root of the repository.
2. Open the **Actions** tab in your GitHub repository.
3. Select **Build Byte Forge VPN Windows Installer** on the left sidebar (it starts automatically on push, or click **Run workflow**).
4. Wait ~3–5 minutes for the green checkmark on **Build & Package Windows x64 Installer**.
5. Scroll down to the **Artifacts** section at the bottom of the workflow summary page and click **`ByteForgeVPN_Setup-Windows-x64`** to download the ZIP containing `ByteForgeVPN_Setup.exe`.

## Project structure

- `.github/workflows/build-windows-installer.yml` — Automated Windows Server 2022 cloud build & QC pipeline
- `installer/ByteForgeVPN.iss` — Inno Setup 6 installer wizard configuration
- `app/` — Application entry point and main window shell
- `core/` — SQLite database layer and URI profile parser/storage
- `ui/` — Qt Widgets page views
- `tests/` — Reserved for future unit, networking, and integration tests
