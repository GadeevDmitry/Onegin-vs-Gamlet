/** @file */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <cmath>
#include <inttypes.h>

#include "Macros.h"
#include "MyString.h"
#include "MyFunc.h"

/**
*   @brief Changes all entries of value "was" to value "will" in byte string
*
*   @param   was [in]   was - character to be replaced
*   @param  will [in]  will - character to replace
*   @param start [in] start - pointer to the first byte of byte string
*   @param     n [in]     n - length of the string
*
*   @return number of replaced characters
*/

int replace(const char was, const char will, char *start, int n)
{
    int OldValues = 0;

    while (n--) {

        if (*start++ == was) {
            start[-1] = will;
            ++OldValues;
        }
    }

    return OldValues;
}

/**
*   @brief Determines if the string includes at least one non-void character.
*   @brief Non-void is the character is not equal to '\n' or '\t' or ' '.
*
*   @param s [in] s - pointer to the first byte of null-terminated byte string
*
*   @return value 1 if string includes at least one non-void character and value 0 else
*/

int at_least_one_NonVoid_char(const char *s)
{
    while (*s) {

        if (!isspace(*s++)) {
            return 1;
        }
    }

    return 0;
}

/**
*   @brief Determines if number less than zero, more than zero or equal to zero
*
*   @param num [in] num - number given to determine
*
*   @return num < 0: -1 | num == 0: 0 | num > 0: 1
*/

int Signum(const double num)
{
    if (num < 0)
        return -1;

    if (num > 0)
        return 1;

    return 0;
}

/**
*   @brief Determines if double number is finite: normal, subnormal or zero, not infinite or NaN
*
*   @param num [in] num - double number
*
*   @return is finite ? 1 : 0
*/

int IsFinite(const double num)
{
    uint64_t int_num = *(uint64_t*)&num;

    uint64_t         infinite_mask = (uint64_t) 0x7ff << 52;
    uint64_t check = infinite_mask & int_num;

    return check != infinite_mask;
}

/**
*   @brief Swaps two variables
*
*   @param   a [in][out] a - pointer to the  first variable
*   @param   b [in][out] b - pointer to the second variable
*   @param len [in]    len - length(in bytes) of memory the pointers are responsible for
*
*   @return nothing
*/

void MySwap(void *a, void *b, const int len)
{
    MyAssert(a != NULL);
    MyAssert(b != NULL);

    unsigned char temp = '\0';

    for (size_t i = 0; i < len; ++i) {

        temp = *((unsigned char *)a + i);
        *((unsigned char *)a + i) = *((unsigned char *)b + i);
        *((unsigned char *)b + i) = temp;
    }
}

/**
*   @brief Compare two double numbers
*
*   @param a [in] a - first number
*   @param b [in] b - second number
*
*   @return (a belong to neighborhood [b - delta; b + delta]) ? 1 : 0
*/

int IsEqual(const double a, const double b)
{
    MyAssert(IsFinite(a));
    MyAssert(IsFinite(b));

    const double delta = 0.00001;

    return fabs(a - b) <= delta;
}

/**
*   @brief Skips all input up to next '\n'
*
*   @param stream [in] stream - input stream
*
*   @return nothing
*/

void clear_input_buff(FILE* const stream)
{
    MyAssert(stream != NULL);

    char ClearingTemp ='#';
    while ((ClearingTemp = getc(stream)) != '\n' && ClearingTemp != EOF) {
        ;
    }

    if (ClearingTemp == '\n') {
        ungetc(ClearingTemp, stream);
    }
}

/**
*   @brief Determines is input stream empty. Stops if a newline character found.
*   @brief empty stream does not include any symbols besides spaces
*
*   @param stream [in] stream - input stream
*
*   @return is empty ? 1 : 0
*/

int is_empty_input_buff(FILE* const stream)
{

    MyAssert(stream != NULL);

    char IsEmptyTemp = ' ';
    while ((IsEmptyTemp = getc(stream)) == ' ' || IsEmptyTemp == '\t') {
        ;
    }

    ungetc(IsEmptyTemp, stream);

    return IsEmptyTemp == '\n' || IsEmptyTemp == EOF;
}

/**
*   @brief Compares two null-terminated byte string backwards lexicography ignoring non-letter characters.
*
*   @param a_ptr [in] (void *) a_ptr - pointer to the first byte of the  first null-terminated byte string
*   @param b_ptr [in] (void *) b_ptr - pointer to the first byte of the second null-terminated byte string
*
*   @return negative value if a < b
*   @return positive value if a > b
*   @return     null value if a = b
*/

int back_only_letter_string_cmp(void *a_ptr, void *b_ptr)
{
    MyAssert(a_ptr != NULL);
    MyAssert(b_ptr != NULL);

    struct word a_word = *(struct word *) a_ptr;
    struct word b_word = *(struct word *) b_ptr;

    char *a = a_word.Begin + a_word.Len - 1;
    char *b = b_word.Begin + b_word.Len - 1;

    while (a >= a_word.Begin && b != a_word.Begin) {

        while (a >= a_word.Begin && !isalpha(*a)) {
            --a;
        }

        while (b >= b_word.Begin && !isalpha(*b)) {
            --b;
        }

        int del = (*a--) - (*b--);

        if (del) {
            return del;
        }
    }

    if (a == a_word.Begin && b == b_word.Begin) {
        return 0;
    }

    if (a == a_word.Begin) {
        return -1;
    }

    return 1;
}

/**
*   @brief Compares two null-terminated byte strings lexicography ignoring non-letter characters
*
*   @param a_ptr [in] (void *) a_ptr - pointer to the first byte of the  first null-terminated byte string
*   @param b_ptr [in] (void *) b_ptr - pointer to the first byte of the second null-terminated byte string
*
*   @return negative value if first string is lexicography less than second string
*   @return positive value if first string is lexicography more than second string
*   @return null if strings are equal
*/

int only_letter_string_cmp(void *a_ptr, void *b_ptr)
{
    MyAssert(a_ptr != NULL);
    MyAssert(b_ptr != NULL);

    char *a = (*(struct word *) a_ptr).Begin;
    char *b = (*(struct word *) b_ptr).Begin;

    while (*a || *b) {

        while (*a && !isalpha(*a)) {
            ++a;
        }

        while (*b && !isalpha(*b)) {
            ++b;
        }

        int del = *a++ - *b++;

        if (del)
            return del;
    }

    return 0;
}

/**
*   @brief Compares two integer
*
*   @param a [in] a - pointer to the  first integer
*   @param b [in] b - pointer to the second integer
*
*   @return negative value if a < b
*   @return positive value if a > b
*   @return     null value if a = b
*/

int DigitCmp(void *a_ptr, void *b_ptr)
{
    MyAssert(a_ptr != NULL);
    MyAssert(b_ptr != NULL);

    int a = *(int *) a_ptr;
    int b = *(int *) b_ptr;

    return a - b;
}

/**
*   @brief Finds the pointer to the element of void array
*
*   @param BeginArray [in] BeginArray - pointer to the first element of void array
*   @param      index [in]      index - number(null numbering) of element to find the pointer
*   @param        len [in]        len - length(in bytes) of memory belonging to one element of the array
*
*   @return pointer to the element
*/

void *GetPtr(void *BeginArray, const int index, const int len)
{
    MyAssert(BeginArray != NULL);

    void *Ptr = (char *)BeginArray + index * len;
    return Ptr;
}

/**
*   @brief Sorts the void array
*
*   @param  data [in][out] data - void array to sort
*   @param   len [in]       len - length(in bytes) of memory belonging to one element of the array
*   @param  left [in]      left - number of the element the sorting begin from
*   @param right [in]     right - number of the element the sorting   end   to
*   @param   cmp [in]       cmp - pointer to the function which compare two variables
*
*   @return nothing
*/

void QuickSort(void *data, int len, int left, int right, int (*cmp) (void * elem1, void * elem2))
{
    //printf("left  = %d, right = %d\n\n", left, right);

    MyAssert(data != NULL);

    /*for (int i = left; i <= right; ++i) {

        printf("data[%d].Begin = %p\n"
               "data[%d].Len = %d\n", i, ((struct word*) data)[i].Begin, i, ((struct word*) data)[i].Len);
    }
    printf("\n"); */

    if (left >= right)
        return;

    int mid = (left + right) / 2;

    MySwap(GetPtr(data, left, len), GetPtr(data, mid, len), len);
    //printf("After swap(%d, %d)\n", left, mid);

    /*for (int i = left; i <= right; ++i) {

        printf("data[%d].Begin = %p\n"
               "data[%d].Len = %d\n",
               i, ((struct word*) data)[i].Begin, i, ((struct word*) data)[i].Len);
    }
    printf("\n"); */

    int cut = left;
    for (int i = left + 1; i <= right; ++i) {

        if ((*cmp)(GetPtr(data, i, len), GetPtr(data, left, len)) <= 0) {

            //printf("cmp(%d, %d): %d < %d\n", i, left, i, left);

            ++cut;

            MySwap(GetPtr(data, cut, len), GetPtr(data, i, len), len);
            //printf("After swap(%d, %d)\n", cut, i);

            /*for (int i = left; i <= right; ++i) {

                printf("data[%d].Begin = %p\n"
                       "data[%d].Len = %d\n",
                       i, ((struct word*) data)[i].Begin, i, ((struct word*) data)[i].Len);
            }
            printf("\n"); */
        }
    }

    MySwap(GetPtr(data, cut, len), GetPtr(data, left, len), len);
    //printf("After swap(%d, %d)\n", cut, left);

    /*for (int i = left; i <= right; ++i) {

        printf("data[%d].Begin = %p\n"
               "data[%d].Len = %d\n",
               i, ((struct word*) data)[i].Begin, i, ((struct word*) data)[i].Len);
    }
    printf("\n"); */

    QuickSort(data, len,    left, cut - 1, cmp);
    QuickSort(data, len, cut + 1,   right, cmp);
}
