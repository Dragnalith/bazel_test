# 2025-03-22

- The official 'Bazel' extension for VS Code works well except it does not succeed in running the debugger
- The extension works with LSP server. I have tried both starpls and bazel-lsp. Both work, but do not provide so much useful info.
- bazel-stack-vscode does not work well except it does support debugging. It should be kept disabled except when debugging because its LSP feature conflict with the official extension.
- You can disable bazel startup message like "Extracting Bazel installation..." using --quiet
- You can disable other bazel output using: --noshow_progress --ui_event_filters=,+error,+fail --show_result=0 --logging=0
- So you can run a target without bazel output with: `bazel --quiet run --noshow_progress --ui_event_filters= --show_result=0 --logging=0 //my/target/label`
- It is possible to use --explain=explain.txt to get more info about what actions have run and why
- It is possible to use -s to display the detail about actions which are run
- By default, `bazel run` run the target with its workspace as working directory.
- despite what the documentation says, --script_path is working on windows and generate regular windows batch_script

# 2025-03-30

- Default rules_cc register default toolchains in the MODULE.bazel of rules_cc.
- It is possible to disable the default C++ toochains registration with env varible `BAZEL_DO_NOT_DETECT_CPP_TOOLCHAIN=1`. You can set it in the .bazelrc: `--repo_env=BAZEL_DO_NOT_DETECT_CPP_TOOLCHAIN=1`.
- The rules_cc module declare to extension repository @local_config_cc and @local_config_cc_toolchains. Because @local_config_cc_toolchains only contain a BUILD, it is considered as a registration-only repo and it is not materialized in the external/ of the exec root
- The default toochain(...) is declared in @local_config_cc_toolchains using cc_toolchain(...) declared in @local_config_cc