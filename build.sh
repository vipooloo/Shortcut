# 1. 生成覆盖率数据文件
lcov -c -d . -o coverage.info

# 2. 过滤掉第三方库、系统库（只看你自己的代码）
lcov --remove coverage.info \
    "/usr/*" \
    "*/third_party/*" \
    "*/build/*" \
    "*/test/*" \
    "*/app/*" \
    "*/src/*JobScLogger.*" \
    -o coverage_filtered.info

# 3. 生成 HTML 报告
genhtml coverage_filtered.info -o coverage_report