#include "unity.h"
#include "fifo_bin.h"
#include <string.h>

void setUp(void)
{
}

void tearDown(void)
{
}

#define TEST_LEN 5

void test_fifo_put_get_byte(void){
    int ret=0;

    /* Create buffer */
    fifo_bin_def(test_fifo,TEST_LEN);

    /* Build test data and save to FIFO */
    uint8_t write_array[TEST_LEN];
    for(uint8_t i=0;i<TEST_LEN;i++){
        write_array[i] = i;
        ret=fifo_bin_put_byte(&test_fifo,i);
        TEST_ASSERT_MESSAGE(ret==0,"Could not put data to fifo");
    }

    /* Check FIFO count*/
    ret=fifo_bin_count(&test_fifo);
    TEST_ASSERT_EQUAL_UINT16(TEST_LEN,ret);

    /* Save data and check FIFO count */
    for(uint8_t i=0;i<TEST_LEN;i++){
        ret=fifo_bin_put_byte(&test_fifo,i);
        TEST_ASSERT_MESSAGE(ret==0,"Could not put data to fifo");
        ret=fifo_bin_count(&test_fifo);
        TEST_ASSERT_EQUAL_UINT16(TEST_LEN,ret);
    }

    /* Get data from FIFO, check data and count */
    uint8_t data=0;
    for(uint8_t i=0;i<TEST_LEN;i++){
        ret=fifo_bin_get_byte(&test_fifo,&data);
        TEST_ASSERT_MESSAGE(ret==0,"Could not put data to fifo");
        TEST_ASSERT_EQUAL_UINT8(i,data);
        ret=fifo_bin_count(&test_fifo);
        TEST_ASSERT_EQUAL_UINT16(TEST_LEN-1-i,ret);
    }

}

void test_fifo_put_get(void){
    int ret=0;

    /*Test buffers*/
    uint8_t expected_array[TEST_LEN+2];
    uint8_t actual_array[TEST_LEN+2];

    /* Create buffer */
    fifo_bin_def(test_fifo,TEST_LEN);

    /* Build test data */
    for(uint8_t i=0;i<TEST_LEN+2;i++){
        expected_array[i] = i;
    }

    /* Put test data to fifo */
    ret=fifo_bin_put(&test_fifo,expected_array,TEST_LEN);
    TEST_ASSERT_MESSAGE(ret==0,"Could not put data to fifo");

    /* Check FIFO count*/
    ret=fifo_bin_count(&test_fifo);
    TEST_ASSERT_MESSAGE(ret==TEST_LEN,"Wrong len of fifo");

    /* Load test data */
    memset(actual_array,0,TEST_LEN+2);
    ret=fifo_bin_get(&test_fifo,actual_array,TEST_LEN);
    TEST_ASSERT_MESSAGE(ret==0,"Could not get data from fifo");

    /* Compare loaded and saved data */
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_array,actual_array, TEST_LEN);

    /* Put test data to fifo - overflow*/
    ret=fifo_bin_put(&test_fifo,expected_array,TEST_LEN+1);
    TEST_ASSERT_MESSAGE(ret!=0,"Save big data");

    /* Load test data */
    ret=fifo_bin_get(&test_fifo,actual_array,TEST_LEN+1);
    TEST_ASSERT_MESSAGE(ret!=0,"Could not get data from fifo");

    /* Load test data */
    memset(actual_array,0,TEST_LEN+2);
    ret=fifo_bin_get(&test_fifo,actual_array,TEST_LEN);
    TEST_ASSERT_MESSAGE(ret==0,"Could not get data from fifo");

    /* Compare loaded and saved data */
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_array,actual_array, TEST_LEN);
}



