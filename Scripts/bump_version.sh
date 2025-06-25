#!/usr/bin/env bash
set -euo pipefail

# Target file
FILE="project_version.json"

# If the file does not exist, create an empty JSON object
[[ -f $FILE ]] || echo '{}' > $FILE

# Extract the current version from the JSON (defaults to 0.0.0)
current_version=$(
  grep -m1 '"version"' "$FILE" \
    | sed -E 's/.*"version"[[:space:]]*:[[:space:]]*"([^"]+)".*/\1/' \
    || echo "0.0.0"
)

# Determine which part to bump: major, minor, or patch (default: patch)
bump=${1:-patch}
IFS='.' read -r maj min pat <<< "$current_version"

case "$bump" in
  major) ((maj++)); min=0; pat=0 ;;
  minor) ((min++)); pat=0 ;;
  patch) ((pat++)) ;;
  *)
    echo "Usage: $0 [major|minor|patch]"
    exit 1
    ;;
esac

new_version="$maj.$min.$pat"

# Gather Git information
commit=$(git rev-parse --short HEAD)
if git diff-index --quiet HEAD --; then
  dirty=false
else
  dirty=true
fi
tag="v$new_version"

# Build metadata: ISO-8601 timestamp and platform
timestamp=$(date +"%Y-%m-%dT%H:%M:%S%z")
platform="$(uname -s)-$(uname -m)"

# Overwrite project_version.json with updated metadata
cat > "$FILE" <<EOF
{
  "projectName": "ProEngine",
  "version": "$new_version",
  "git": {
    "commit": "$commit",
    "tag": "$tag",
    "dirty": $dirty
  },
  "build": {
    "timestamp": "$timestamp",
    "platform": "$platform"
  }
}
EOF

echo "project_version.json updated to $new_version"

# Create an annotated Git tag for the new version
git tag -a "$tag" -m "$tag"
echo "Created Git tag $tag"
