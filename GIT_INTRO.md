# Git introduction

Git keeps a local history of your project. A **commit** records a snapshot of the staged files; Git can show the changes between snapshots. A **branch** is a movable name pointing to a commit. A **remote** is a named connection to another repository, for example on GitHub.

Run these commands in the VS Code terminal. Lines starting with `#` are comments, not commands. The examples use `david` as the branch name; replace it with your own name consistently.

## 1. Get the course files

```bash
# Copy the repository and its history to your computer. Do this only once.
git clone https://github.com/davemod/2610-JUCE-1.git

# Enter the new repository folder. Run the remaining commands from here.
cd 2610-JUCE-1

# Show your branch, changed files, and status relative to its upstream branch.
git status

# List the configured remotes and their addresses. clone names its remote origin.
git remote -v
```

An **upstream branch** is the branch Git uses as the default for comparing and synchronizing your local branch. `git status` compares against your last fetched information; it does not contact GitHub to check for newer commits.

## 2. Inspect local and remote history

```bash
# Show the commits reachable from your current branch, newest first.
git log

# Download new commits and update your local view of remote branches.
# This does not merge them into your branch or change your working files.
git fetch origin

# Show history across all local and remote-tracking branches.
git log --all --oneline --graph --decorate

# List local branches. The * marks your current branch.
git branch

# Also list remote-tracking branches, such as remotes/origin/main.
git branch -a
```

If a log opens in a pager, press `q` to return to the terminal. Remote-tracking branches such as `origin/main` represent your local view of the remote at the last fetch.

## 3. Create and switch branches

Start with a clean working tree (`git status`) so your changes do not accidentally follow you to another branch.

```bash
# Create a branch at the current commit. This does not switch to it yet.
git branch david

# Switch to that branch.
git switch david

# Check which branch you are on before editing.
git status
```

Alternatively, `git switch -c david` creates and switches in one command; use it instead of the two commands above. If the branch already exists, just use `git switch david`. The older command `git checkout david` also switches to an existing branch.

## 4. Review, stage, and commit your changes

The usual order is **edit → save → build/run → review → stage → commit**. Saving a file does not create a commit. Staging selects the version of a file to include in the next commit.

```bash
# Show changed and untracked files.
git status

# Review changes to tracked files that have not been staged yet.
git diff

# Stage one specific file. Paths are relative to your current directory.
git add session-02/source/main.cpp

# Review exactly what will go into the commit.
git diff --staged

# Record the staged changes in your local history.
git commit -m "Solve Session 02 tempo validation and classification"

# Confirm what, if anything, remains uncommitted.
git status
```

`git diff` does not display the contents of new, untracked files; open those files to review them. If you edit a file after staging it, run `git add` again to include the newer version.

These are alternatives or corrections to use when needed:

```bash
# Stage all changes under the current directory, including new files and deletions.
# Use only after checking that every change belongs in your commit.
# The dot means "the current directory".
git add .

# Unstage changes under the current directory. Your file edits are kept.
git restore --staged .
```

If your first commit asks for your identity, configure it for this repository, using your own details, then retry the commit:

```bash
git config user.name "Your Name"
git config user.email "your-email@example.com"
```

## 5. Push your commits

Committing saves history locally; pushing shares commits with a remote. You need write access to the destination repository. If you cannot push to the course repository, use the student repository or fork agreed in class; cloning alone does not grant write access.

```bash
# First push of your branch: publish it and set its upstream branch.
git push -u origin david

# For later pushes from the same branch, the upstream is already configured.
git push
```

Push sends commits, not uncommitted edits. If a push is rejected because the remote has newer commits, fetch and inspect the history before integrating them. Do not force-push to solve this.

## 6. Pull updates into the current branch

Check your branch and commit your current work before pulling. With an upstream configured:

```bash
git status

# Fetch, then move your current branch forward to its upstream if possible.
git pull --ff-only
```

A **fast-forward** is possible when your branch has no commits that diverge from its upstream. `--ff-only` stops if integrating the two histories would require a merge or rebase. If it stops, inspect `git log --all --oneline --graph --decorate` and discuss the next step in class.

Plain `git pull` also fetches first, then integrates changes according to your Git configuration and options, potentially using a merge or rebase. The explicit `--ff-only` option keeps this introductory workflow predictable.

Pulling while on `david` with upstream `origin/david` updates from that branch; it does not automatically bring in new course material from `origin/main`. Follow the workflow agreed in class for incorporating teacher updates. Independent student solutions do not need to be merged into the teacher's reference solution.
