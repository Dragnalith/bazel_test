load("@rules_cc//cc:defs.bzl", "cc_binary")

def _slg_binary_impl(ctx):
    # Get the input file
    input_file = ctx.file.src
    
    # Determine output file extension based on language
    output_ext = ".py" if ctx.attr.language == "python" else ".cpp"
    output_file = ctx.actions.declare_file(ctx.label.name + output_ext)
    
    # Build the command line arguments
    args = ctx.actions.args()
    args.add(input_file)
    if ctx.attr.language == "python":
        args.add("--python")
    else:
        args.add("--cpp")
    args.add("-o", output_file)
    
    # Run the compiler
    ctx.actions.run(
        executable = ctx.executable._compiler,
        arguments = [args],
        inputs = [input_file],
        outputs = [output_file],
        mnemonic = "SlgCompile",
        progress_message = "Compiling %s" % input_file.short_path,
    )
    
    # Return the output file
    return [DefaultInfo(files = depset([output_file]))]

slg_binary = rule(
    implementation = _slg_binary_impl,
    attrs = {
        "src": attr.label(
            allow_single_file = [".slg"],
            mandatory = True,
        ),
        "language": attr.string(
            values = ["cpp", "python"],
            default = "cpp",
        ),
        "_compiler": attr.label(
            default = "//src/simplang:simplang-compiler",
            executable = True,
            cfg = "exec",
        ),
    },
) 