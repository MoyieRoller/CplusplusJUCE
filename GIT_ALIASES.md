# Git shell aliases

These four shell aliases are shortcuts for common Git commands:

```bash
alias gl='git log --graph --oneline'
alias gla='gl --all'
alias gs='git status'
alias gd='git diff'
```

Use `gl` for the current branch history, `gla` for history across all branches, `gs` for the working tree status, and `gd` for unstaged changes.

## Where to put them

Add the four lines to the startup file for the shell you use. If that file already exists, append the aliases to it.

| Platform and shell | Startup file |
| --- | --- |
| macOS, zsh (default) | `~/.zshrc` |
| macOS or Linux, bash | `~/.bashrc` (on macOS, a login shell may use `~/.bash_profile`) |
| Linux, zsh | `~/.zshrc` |
| Windows, Git Bash | `~/.bashrc` in the Git Bash home directory, usually `C:\Users\YourName\.bashrc` |

These `alias` lines use Bash/Zsh syntax. In Windows, use Git Bash; they do not work as written in Command Prompt or PowerShell.

After saving, open a new terminal, or reload the file in the current terminal. For example, use `source ~/.zshrc` in zsh or `source ~/.bashrc` in bash.
