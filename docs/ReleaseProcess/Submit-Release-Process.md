## How to submit a new tag or release version

1. Give permission to bump_version.sh script `chmod +x bump_version.sh`
2. Run the bump_version.sh script `./bump_version.sh`
3. Commit the modification with the following message: `git commit -sm "Bump to version 0.0.2`
4. Push the generated tag `git push origin v0.0.2`
5. Create a new release in the github page selecting the generated tag