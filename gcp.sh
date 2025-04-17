#!/bin/bash

# --------------------------------------------
# 💣 eChess Git Commit Console - Built by Agent
# --------------------------------------------

# Enable colors
RED='\033[1;31m'
GREEN='\033[1;32m'
YELLOW='\033[1;33m'
CYAN='\033[1;36m'
NC='\033[0m'

echo -e "${CYAN}🧠 Booting eChess Git Commit System...${NC}"

# 1. Check if inside a git repo
if ! git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
    echo -e "${RED}❌ ERROR: This is not a Git repo. Navigate to a Git-initialized project.${NC}"
    exit 1
fi

# 2. Check if .git directory exists
if [ ! -d ".git" ]; then
    echo -e "${RED}❌ ERROR: Missing .git folder — this isn't a valid repo.${NC}"
    exit 1
fi

# 3. Check for unstaged changes
if [ -z "$(git status --porcelain)" ]; then
    echo -e "${YELLOW}⚠️  No changes to commit. Working directory clean.${NC}"
    exit 0
fi

# 4. Check SSH Authentication to GitHub
GIT_USER=$(git config user.name)
GIT_EMAIL=$(git config user.email)

echo -e "${CYAN}🔐 Checking SSH authentication with GitHub...${NC}"
ssh -T git@github.com -o StrictHostKeyChecking=no 2>&1 | grep -q "successfully authenticated"
if [ $? -eq 1 ]; then
    echo -e "${RED}❌ SSH Auth Failed. Make sure your key is added via ssh-agent and GitHub.${NC}"
    echo -e "${YELLOW}Tip: Run ${NC}ssh-add ~/.ssh/YOUR_KEY${YELLOW} if needed.${NC}"
    exit 1
fi

echo -e "${GREEN}✅ SSH Authentication successful. GitHub connection verified.${NC}"
echo -e "${CYAN}👤 Git User: ${GREEN}$GIT_USER <$GIT_EMAIL>${NC}"

# 5. Stage all changes
echo -e "${CYAN}📦 Staging all changes...${NC}"
git add .

# 6. Prompt for commit message
echo -ne "${CYAN}💬 Enter your commit message: ${NC}"
read -r COMMIT_MSG

if [ -z "$COMMIT_MSG" ]; then
    echo -e "${RED}❌ You must enter a commit message. Aborting.${NC}"
    exit 1
fi

# 7. Perform commit
git commit -m "$COMMIT_MSG"

# 8. Surprise bonus: timestamped log 📓
LOGFILE=".eChess_commit_log.txt"
echo "$(date '+%Y-%m-%d %H:%M:%S') | $GIT_USER | $COMMIT_MSG" >> "$LOGFILE"
echo -e "${GREEN}📝 Logged commit to $LOGFILE${NC}"

# 9. Optional: Show git status after commit
echo -e "${CYAN}📋 Git Status:${NC}"
git status -sb

# 10. Ask to push
echo -ne "${CYAN}🚀 Push to origin? [y/N]: ${NC}"
read -r PUSH_CONFIRM

if [[ "$PUSH_CONFIRM" =~ ^[Yy]$ ]]; then
    git push origin "$(git branch --show-current)"
    echo -e "${GREEN}🔥 Code launched to GitHub. Target neutralized.${NC}"
else
    echo -e "${YELLOW}⏹ Push skipped. Your changes are local only.${NC}"
fi

echo -e "${CYAN}🧠 Commit process complete. Stay lethal, Colonel.${NC}"

