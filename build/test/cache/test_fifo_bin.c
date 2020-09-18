#include "build/temp/_test_fifo_bin.c"
#include "src/fifo_bin.h"
#include "C:/tools/ruby27/lib/ruby/gems/2.7.0/gems/ceedling-0.30.0/vendor/unity/src/unity.h"




void setUp(void)

{

}



void tearDown(void)

{

}







void test_fifo_put_get_byte(void){

    int ret=0;





    uint8_t test_fifo_buf[5];fifo_bin_t test_fifo = {.NewI=0,.OldI=0,.Count=0,.Total_SZ=5,.Buffer=test_fifo_buf};;





    uint8_t write_array[5];

    for(uint8_t i=0;i<5;i++){

        write_array[i] = i;

        ret=fifo_bin_put_byte(&test_fifo,i);

        do {if ((ret==0)) {} else {UnityFail( ((("Could not put data to fifo"))), (UNITY_UINT)((UNITY_UINT)(26)));}} while(0);

    }





    ret=fifo_bin_count(&test_fifo);

    UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((5)), (UNITY_INT)(UNITY_UINT16)((ret)), (

   ((void *)0)

   ), (UNITY_UINT)(31), UNITY_DISPLAY_STYLE_UINT16);





    for(uint8_t i=0;i<5;i++){

        ret=fifo_bin_put_byte(&test_fifo,i);

        do {if ((ret==0)) {} else {UnityFail( ((("Could not put data to fifo"))), (UNITY_UINT)((UNITY_UINT)(36)));}} while(0);

        ret=fifo_bin_count(&test_fifo);

        UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((5)), (UNITY_INT)(UNITY_UINT16)((ret)), (

       ((void *)0)

       ), (UNITY_UINT)(38), UNITY_DISPLAY_STYLE_UINT16);

    }





    uint8_t data=0;

    for(uint8_t i=0;i<5;i++){

        ret=fifo_bin_get_byte(&test_fifo,&data);

        do {if ((ret==0)) {} else {UnityFail( ((("Could not put data to fifo"))), (UNITY_UINT)((UNITY_UINT)(45)));}} while(0);

        UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((i)), (UNITY_INT)(UNITY_UINT8 )((data)), (

       ((void *)0)

       ), (UNITY_UINT)(46), UNITY_DISPLAY_STYLE_UINT8);

        ret=fifo_bin_count(&test_fifo);

        UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((5 -1-i)), (UNITY_INT)(UNITY_UINT16)((ret)), (

       ((void *)0)

       ), (UNITY_UINT)(48), UNITY_DISPLAY_STYLE_UINT16);

    }



}



void test_fifo_put_get(void){

    int ret=0;





    uint8_t expected_array[5 +2];

    uint8_t actual_array[5 +2];





    uint8_t test_fifo_buf[5];fifo_bin_t test_fifo = {.NewI=0,.OldI=0,.Count=0,.Total_SZ=5,.Buffer=test_fifo_buf};;





    for(uint8_t i=0;i<5 +2;i++){

        expected_array[i] = i;

    }





    ret=fifo_bin_put(&test_fifo,expected_array,5);

    do {if ((ret==0)) {} else {UnityFail( ((("Could not put data to fifo"))), (UNITY_UINT)((UNITY_UINT)(70)));}} while(0);





    ret=fifo_bin_count(&test_fifo);

    do {if ((ret==5)) {} else {UnityFail( ((("Wrong len of fifo"))), (UNITY_UINT)((UNITY_UINT)(74)));}} while(0);





    memset(actual_array,0,5 +2);

    ret=fifo_bin_get(&test_fifo,actual_array,5);

    do {if ((ret==0)) {} else {UnityFail( ((("Could not get data from fifo"))), (UNITY_UINT)((UNITY_UINT)(79)));}} while(0);





    UnityAssertEqualIntArray(( const void*)((expected_array)), ( const void*)((actual_array)), (UNITY_UINT32)((5)), (

   ((void *)0)

   ), (UNITY_UINT)(82), UNITY_DISPLAY_STYLE_UINT8, UNITY_ARRAY_TO_ARRAY);





    ret=fifo_bin_put(&test_fifo,expected_array,5 +1);

    do {if ((ret!=0)) {} else {UnityFail( ((("Save big data"))), (UNITY_UINT)((UNITY_UINT)(86)));}} while(0);





    ret=fifo_bin_get(&test_fifo,actual_array,5 +1);

    do {if ((ret!=0)) {} else {UnityFail( ((("Could not get data from fifo"))), (UNITY_UINT)((UNITY_UINT)(90)));}} while(0);





    memset(actual_array,0,5 +2);

    ret=fifo_bin_get(&test_fifo,actual_array,5);

    do {if ((ret==0)) {} else {UnityFail( ((("Could not get data from fifo"))), (UNITY_UINT)((UNITY_UINT)(95)));}} while(0);





    UnityAssertEqualIntArray(( const void*)((expected_array)), ( const void*)((actual_array)), (UNITY_UINT32)((5)), (

   ((void *)0)

   ), (UNITY_UINT)(98), UNITY_DISPLAY_STYLE_UINT8, UNITY_ARRAY_TO_ARRAY);

}
