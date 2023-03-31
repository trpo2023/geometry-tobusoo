#include <ctest.h>
#include <stdio.h>

#include <libgeometry/geom_parser.h>
#include <libgeometry/lexer.h>

CTEST(lexer, count_char)
{
    const char ch = '-';
    const char exp_ch = '-';
    int cnt = 0;

    int result = count_char(ch, exp_ch, &cnt);

    int expected = 1;
    ASSERT_EQUAL(expected, result);

    result = count_char(ch, exp_ch, &cnt);
    expected = 2;
    ASSERT_EQUAL(expected, result);
}

CTEST(lexer, to_lower_string)
{
    char string1[7] = "Circle";
    char string2[7] = "CIRCLE";
    char string3[8] = "CirCle";
    char string4[9] = "TRIangle";
    char expected1[7] = "circle";
    char expected2[9] = "triangle";

    to_lower_string(string1);
    to_lower_string(string2);
    to_lower_string(string3);
    to_lower_string(string4);

    ASSERT_STR(string1, expected1);
    ASSERT_STR(string2, expected1);
    ASSERT_STR(string3, expected1);
    ASSERT_STR(string4, expected2);
}

CTEST(lexer, read_str_number)
{
    char temp[25] = {0};
    int column = 0;
    FILE* file = fopen("test/read_str_number.txt", "r");
    int result = read_str_number(temp, &column, file);
    int expected = 0;
    ASSERT_STR(temp, "3.5");
    ASSERT_EQUAL(expected, result);

    char temp1[25] = {0};
    int result1 = read_str_number(temp1, &column, file);
    int expected1 = 0;
    ASSERT_STR(temp1, "-1.5");
    ASSERT_EQUAL(expected1, result1);

    char temp2[25] = {0};
    int result2 = read_str_number(temp2, &column, file);
    int expected2 = 0;
    ASSERT_STR(temp2, "0");
    ASSERT_EQUAL(expected2, result2);

    char temp4[25] = {0};
    int result4 = read_str_number(temp4, &column, file);
    int expected4 = 0;
    ASSERT_STR(temp4, "4545454");
    ASSERT_EQUAL(expected4, result4);

    char temp3[25] = {0};
    int result3 = read_str_number(temp3, &column, file);
    int expected3 = ER_NOT_DOUBLE;
    ASSERT_EQUAL(expected3, result3);

    file->_IO_read_ptr++;
    char temp5[25] = {0};
    int result5 = read_str_number(temp5, &column, file);
    int expected5 = ER_BACKSLASH;
    ASSERT_EQUAL(expected5, result5);
}

CTEST(lexer, unexpect_char)
{
    char ch = ',';
    char expected_ch = '(';
    FILE* file = fopen("unexpect_char_test.txt", "w");
    bool result = unexpect_char(ch, expected_ch, file);
    remove("unexpect_char_test.txt");
    bool expect = false;
    ASSERT_EQUAL(expect, result);

    char ch1 = ',';
    char expected_ch1 = ',';
    FILE* file1 = fopen("unexpect_char_test.txt", "w");
    bool result1 = unexpect_char(ch1, expected_ch1, file1);
    remove("unexpect_char_test.txt");
    bool expect1 = true;
    ASSERT_EQUAL(expect1, result1);
}