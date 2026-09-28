# Source sync and Git

Use this repository folder as the Syncthing folder root. Keep build workspaces
outside it where possible. `.gitignore` controls Git; `.stignore` controls
Syncthing. They use different syntax and must be updated together when adding
new output locations. Git directory rules end in `/`; Syncthing directory rules
normally do not, so the directory itself is excluded too.

On each computer, place `.stignore` in the folder before starting its first
sync. All Syncthing patterns are in that one file, with no includes. Syncthing
does not transfer `.stignore` itself: copy it manually to each computer after
changing the rules. Replace any previous include-based loader with this file.

Source files, XSA handoffs, build configuration and portable VS Code settings
sync normally. Git's `.git` metadata, build output and tool caches stay local.
Syncthing markers/version archives are not committed. Conflict copies remain
visible so they can be reviewed rather than silently hidden.

Keep Git history on the primary computer, or exchange committed history using
Git remotes. Use one computer at a time for editing and wait for sync to finish
before switching. Branch checkouts also change files and will be synchronized.

Ignore rules do not remove files already copied to another computer. If `.git`
has already synced, it remains there but future changes are excluded. Do not
assume that copy has a consistent history; verify it before using it as a repo.
This setup does not change Syncthing device connections or versioning settings.
