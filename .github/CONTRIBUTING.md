# Option Pricer

## Repository Assumptions and Guidelines

This repository follows a structured workflow to ensure code quality, stability, and consistency.
Below are the key assumptions and guidelines that contributors should follow:

### Branching Strategy

1. **Main Branch (`main`)**:
    - The `main` branch contains stable, production-ready code.
    - Direct pushes to the `main` branch are **not allowed**. All changes must be introduced via pull requests (PRs).
    - Before merging into `main`, all PRs must pass the CI/CD pipeline and receive necessary approvals.

2. **Development Branch (`dev`)**:
    - The `dev` branch is the primary branch for ongoing development.
    - All feature branches should be created from and merged back into `dev`.
    - Pushing to `dev` also triggers the CI/CD pipeline to ensure that ongoing development does not introduce errors.

3. **Feature Branches (`feature/*`)**:
    - Feature branches are used for developing new features and should be created from `dev`.
    - Once a feature is complete and tested, it should be merged back into `dev`.
    - Example: `feature/your-feature-name`.

4. **Release Branches (`release/*`)**:
    - Release branches are created from `dev` when preparing for a new production release.
    - After final testing and necessary changes, release branches are merged into both `main` and `dev`.
    - Example: `release/1.0`.

5. **Hotfix Branches (`hotfix/*`)**:
    - Hotfix branches are created from `main` to address critical issues found in production.
    - After fixing the issue, hotfix branches are merged into both `main` and `dev`.
    - Example: `hotfix/issue-name`.

### CI/CD Pipeline

1. **Continuous Integration/Continuous Deployment (CI/CD)**:
    - The CI/CD pipeline is triggered by pushes to any branch except `main`.
    - The pipeline includes steps such as building the project, running tests, and static code analysis (e.g., using `clang-tidy`).

2. **Pull Requests and CI/CD**:
    - To merge changes into `main`, you must create a pull request (PR).
    - The CI/CD pipeline automatically runs for each PR to validate the changes.
    - Only after the CI/CD pipeline passes and the PR is approved can it be merged into `main`.

### Workflow Guidelines

1. **Creating a Feature Branch**:
   - Ensure you're on the `dev` branch:
     ```sh
     git checkout dev
     git pull origin dev
     git checkout -b feature/your-feature-name
     ```

2. **Committing Changes**:
   - Add and commit your changes with descriptive messages:
     ```sh
     git add .
     git commit -m "Add new feature"
     ```

3. **Pushing Changes**:
   - Push the feature branch to the remote repository to trigger the CI/CD pipeline:
     ```sh
     git push -u origin feature/your-feature-name
     ```

4. **Creating a Pull Request**:
   - Navigate to the repository on GitHub.
   - Click on the "Pull requests" tab and "New pull request".
   - Select `main` as the base branch and your feature branch as the compare branch.
   - Fill in the title and description and click "Create pull request".

5. **Merging a Pull Request**:
   - Ensure all CI/CD checks pass and the PR is approved.
   - Merge the PR on GitHub to integrate changes into `main`.
   - Pull changes from origin/main to local main by calling `git pull` inside main.

## Example workflows

### Feature Development Workflow for `dev` Branch

1. **Create a feature branch from `dev`**:
```sh
git checkout -b feature/your-feature dev
```
2. **Make changes, add and commit**:
```
git add .
git commit -m "Add new feature"
```
3. **Push the feature branch to remote to trigger the CI/CD pipeline**:
```sh
git push -u origin feature/your-feature
```
4. **After CI/CD passes, merge the feature branch into dev**:
```sh
git checkout dev
git pull origin dev  # get latest updates
git merge feature/your-feature
```
5. **Push the updated `dev` branch**:
```sh
git push -u origin dev
```
6. **Delete the feature branch on local and on remote**:
```sh
git branch -d feature/your-feature
git push origin --delete feature/your-feature
```

## Contact
For any questions and/or contributions,
please contact Anthony Makarewicz at anthonymakarewicz@gmail.com