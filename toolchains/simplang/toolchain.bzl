"""
Define Simplang Toolchain rules
"""

SimplangInfo = provider("SimplangInfo Toolchain Info",
    fields = {
        "compiler_path": "path to the simplang compiler",
    },
)

def _toolchain_impl(ctx):
    toolchain_info = platform_common.ToolchainInfo(
        simplang_info = SimplangInfo(
            compiler_path = ctx.attr.compiler_path[DefaultInfo].files.to_list()[0]
        )
    )
    return [toolchain_info]

simplang_toolchain = rule(
    implementation = _toolchain_impl,
    attrs = {
        "compiler_path": attr.label()
    },
)