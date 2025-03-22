# 2025-03-22

- The official 'Bazel' extension for VS Code works well except it does not succeed in running the debugger
- The extension works with LSP server. I have tried both starpls and bazel-lsp. Both work, but do not provide so much useful info.
- bazel-stack-vscode does not work well except it does support debugging. It should be kept disabled except when debugging because its LSP feature conflict with the official extension.
- You can disable bazel startup message like "Extracting Bazel installation..." using --quiet
- You can disable other bazel output using: --noshow_progress --ui_event_filters=,+error,+fail --show_result=0 --logging=0
- So you can run a target without bazel output with: `bazel --quiet run --noshow_progress --ui_event_filters= --show_result=0 --logging=0 //my/target/label`
- It is possible to use --explain=explain.txt to get more info about what actions have run and why
- It is possible to use -s to display the detail about actions which are run