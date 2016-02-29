/**
 * Company	: ThyssenKrupp PDC
 * @file	: check_instruction_post_2.s
 * @purpose     :
 * @brief	: Instruction POST test for IEC61508 verification on ETC-01 mainboard.
 * @version	: 1.0
 * @date	: 11-April-2014
 * @author	: Paul
*/
;����������
  SECTION .text:DATA  ;REORDER:NOROOT    ;:ROOT
;MyData1
;  DCB 0 ; Destination of calculation result
;  DCD 0xE000E400

;  EXPORT  __Vectors  
  ;//DCD��Define Const DWORD  
;__Vectors       DCD     __initial_sp              ; Top of Stack      

;//[WEAK]ѡ������������ͬ����������ڸñ�ű����á�  
;//EXPORT��ʾ�������ñ�ſ���Ϊ�ⲿ�ļ����á�  
;//IMPORT֪ͨ������Ҫʹ�õı���������ļ���  
;//ʹ�á�=����ʾLDRĿǰ��αָ��Ǳ�׼ָ� 

  SECTION .text:CODE

  PUBLIC  _ITT_Arithmetic  
  PUBLIC  _TBB_Arithmetic  

  EXTERN  InstCheckPOST_struct

;/* Test pattern definition */ 
;pattern1 EQU  0x50000000
;__VALID_PRI_LBIT    EQU     0x03                                  ;/* ���ȼ������Чλ,��оƬ��� */
;__SYSTICK_PRI       EQU     0xFF                                  ;/* SysTick�����ȼ���Ĭ��Ϊ��� */
;__INT_BASE_PRI      EQU     (0x02 << __VALID_PRI_LBIT)            ;/* �������ȼ���ֵ-���㣬Ĭ��Ϊ2*/

;;;;;;;;;;;----NO TEST----;;;;;;;;;;;
;BKPT BL CPS CPY LDRSH LDRSB LDRH
;STMIA STRB STRH UXTB CLZ ISB 
;MLA.W MLS.W MOVT.W MOVW.W 
;TEQ.W TST.W 


;TBB
;TBB.W [Rn, Rm] ; PC+= Rn[Rm]*2  ;Rn ָ����ת��Ļ�ַ��Rm ���������Ԫ�ص��±�
;TBB.W [pc, r0] ; ִ�д�ָ��ʱ��PC ��ֵ���õ���branchtable
;branchtable
;  DCB ((dest0 �C branchtable)/2) ; ע�⣺��Ϊ��ֵ��8 λ�ģ���ʹ��DCB ָʾ��
;  DCB ((dest1 �C branchtable)/2)
;  DCB ((dest2 �C branchtable)/2)
;  DCB ((dest3 �C branchtable)/2)
;dest0
;  MOV R1, #0  ; r0 = 0 ʱִ��
;dest1
;  MOV R1, #1  ; r0 = 1 ʱִ��
;dest2
;  MOV R1, #2  ; r0 = 2 ʱִ��
;dest3
;  MOV R1, #3  ; r0 = 3 ʱִ��  

;/**************************************/
;/* START of the _TBB_Arithmetic test  3*/
;/**************************************/
_TBB_Arithmetic
;/* Push ALL registers to stack */
  push {r0-r12,r14}
  
  MOV R0, #1
  TBB.W   [PC, R0]               ; ִ�д�ָ��ʱ��PC ��ֵ���õ���__SwiFunction                                     
    DATA
__DcbFunction
    DCB     ( (Donothing - __DcbFunction) / 2 )      ;0   ;ע�⣺��Ϊ��ֵ��8 λ�ģ���ʹ��DCB ָʾ��
    DCB     ( (Addone  - __DcbFunction) / 2 )        ;1   ;DCB ����һƬ�������ֽڴ洢��Ԫ����ֵ��ʼ��
Tbhw      ;label
  MOV R0, #0    
      
  TBH.W   [PC, R0,LSL #1]                                                    
    DATA
DcbFunctionAdd       
    DC16    ( (Addtwo  - DcbFunctionAdd) / 2 )               ; 0           
;  DC16    ( (ClrvicIrqFunc    - SwiFunctionAdd) / 2 )     ; 1            


Tbbreturn  ;label
  THUMB    ;ָʾ������32 λThumb-2 ָ��

;CPS <effect>, <iflags>    ;�ı䴦����״̬  CM3 û�С�A��λ

;CPY ��һ���Ĵ�����ֵ��������һ���Ĵ�����
  MOV R1, #1
  CPY R0, R1
  CMP R0, R1
  BNE _TBB_Arithmetic_test_fail
  
;LDRSH  ;���ذ��ֵ��Ĵ������ٴ�������չ��32λ
  ;LDRSH sXi, [PssX, #2]        
  ;LDRSH sXr, [PssX]
  ;ADD PssX, PssX, offset

;LDRSB ;�Ӵ洢���м����ֽڣ��پ�����������չ��洢��һ���Ĵ�����
 ;LDRSB    R0,[R0, #+0]

;LDRH  ;�Ӵ洢���м��ذ��ֵ�һ���Ĵ�����


;STMIA ;�洢����֣������ڴ洢��������ַ�Ĵ��� 
;STRB  ;�洢�Ĵ����е� ���ֽ�
 ;LDR      R1,??DataTable74  
 ;STRB     R0,[R1, #+0]

;STRH  ;�洢�Ĵ����е� �Ͱ���
  ;STRH     R5,[R0, #+0]

;STRH�������ݴ洢ָ�
;STRB�ֽ����ݴ洢ָ��

;UXTB 
;CLZ 
;ISB 
;MLA.W 
;MLS.W 
;MOVT.W 
;MOVW.W 
;TEQ.W 
;TST.W 

;ADC ����λ�ӷ�ָ��
;ADCָ����ɵĹ����ǽ�������1���ϲ�����2���ټ��ϱ�־λC��ֵ������͵�Ŀ�ļĴ���
;ADDS  R1�� R1��#10   ;���Ӱ���־λ
;ADD   R1�� R1�� R2   ;�����Ӱ���־λ
;ADD   R3�� R1�� R2�� LSL #2   ; R3 = R1 + ( R2 << 2 )

  MOV R0, #0xff
  MOV R1, #0xff
  ADDS R1, R0, R1
  CMP R1, #510
  ADC R0, R0, R1  ;766

  MOV R0, #0
  MOV R1,#0xffff
  ADD R0, R0, R1  ;65535
  ADC R0, R1, #0  ;65536
  

;SBCָ����ɵĹ����ǽ�������1��ȥ������2���ټ�ȥ��־λC��ȡ��ֵ������͵�Ŀ�ļĴ���
;SUB  R0�� R1�� #256  ��R0 �� R1 �� 256 �� �����Ӱ���־λ
;SUBS  R0�� R2��R3��LSL #1 ��R0 = R2 �� ( R3 <<1 )�����Ӱ���־λ
;SUB  SP �� #380   ��SP �� SP �� 380
;SBC  R0�� R1�� R2 ��R0 �� R1 �� R2 �� !C
;RSC  R0�� R1�� R2 ��R0 �� R2 �� R1 �� !C

  MOV R1, #0
  MOV R2, #0xFF
  SUBS R0, R1, #256
  CMP R0, #256
  SBC R0, R1, R2    ;FFFF FF01
  
  MOV R1, #0
  MOV R2, #0xFF
  SUB R0, R1, #256  ;FFFF FF00
  SBC R0, R1, R2    ;FFFF FF01

;  RSC R0,R1,R2      ;R0 �� R2 �� R1 �� !C not available in THUMB mode

;  ADRL R0, ThumbFun + 1    ;���ɷ�֧��ַ�������λΪ 1
;  BLX R0                    ;��ת��R0��ָ���ĵ�ַ�����л���������Thumb����״̬

  ;BL��������  Ŀ���ַ
  ;ָ������ת֮ǰ�����ڼĴ���R14�б��浱ǰ����һ��ָ��ĵ�ַ��
  ;��ˣ�����ͨ����R14���¼��ص�PC�У������ص���תָ��֮�������ָ�ִ�С�
  ;��ָ����ʵ���ӳ�����õ�һ�ֳ����ֶ�

  ;BKPT  16λ��������
  ;���ܣ����ڲ�������ϵ��жϣ�ִ��ʱ�ж�����ָ�������Ӧ�ĵ����ӳ���

  LDR  R1, =InstCheckPOST_struct     /*COUNT*/
  LDRB R0, [R1]                      /*LDRB*/
  ADD  R0,R0,#0x01
  STR  R0, [R1]

_TBB_Arithmetic_test_fail
;/* Pop the stack back */
  pop {r0-r12,r14}
;/* Branch back */
  bx  lr
;/**************************************/
;/* END of the _TBB_Arithmetic test     */
;/**************************************/


ThumbFun
;Thumb���ָ��
  MVN R1, #0xFF    ;R1 �� 0X FFFF FF00
  ;MVN R1, R2
  MOV PC, R14      ;���Ĵ���R14��ֵ���͸�PC�������ӳ��򷵻ء�

;/*********************************************************************************************************
;** Function name:           Donothing
;** Descriptions:            �����ô˺���
;** input parameters:        none
;** output parameters:       none
;** Returned value:          none                          
;*********************************************************************************************************/
Donothing  ; r0 = 0 ʱִ��
    BX LR

;/*********************************************************************************************************
;** Function name:           Addone
;** Descriptions:            ���ô˺�������һ
;** input parameters:        none
;** output parameters:       none
;** Returned value:          none                          
;*********************************************************************************************************/
Addone     ; r0 = 1 ʱִ��
  LDR  R1, =InstCheckPOST_struct     /*COUNT*/
  LDRB R0, [R1]                      /*LDRB*/
  ADD  R0,R0,#0x01
  STR  R0, [R1]
  B Tbhw
    
;/*********************************************************************************************************
;** Function name:           Addtwo
;** Descriptions:            ���ô˺���������һ
;** input parameters:        none
;** output parameters:       none
;** Returned value:          none                          
;*********************************************************************************************************/
Addtwo     ; r0 = 0 ʱִ��
  LDR  R1, =InstCheckPOST_struct     /*COUNT*/
  LDRB R0, [R1]                      /*LDRB*/
  ADD  R0,R0,#0x01
  STR  R0, [R1]
  B Tbbreturn

;/**************************************/
;/* START of the _ITT_Arithmetic test     5*/
;/**************************************/
_ITT_Arithmetic
;/* Push ALL registers to stack */
  push {r0-r12,r14}

;if  ( a > b )  a++;
;else  b++;
  ;MOV R0, #0
  ;MOV R1, #1
  ;CMP  R0, R1  ; R0��R1�Ƚϣ���R0-R1�Ĳ��� ;ָ���һ��������ȥ�ڶ��������������洢�����ֻ����������־λ
  ;Conditional instruction is only allowed inside IT block
  ;ADDHI  R0, R0, #1  ;��R0 > R1, ��R0 = R0 + 1
  ;ADDLS  R1, R1, #1  ; ��R0 <= R1, ��R1 = R1 + 1

;if (r0==r1)
;{r3=r4+r5; r3=r3/2;}
;else
;{r3=r6+r7; r3=r3/2;}

  MOV R0, #1
  MOV R1, #2
  MOV R4, #5
  MOV R5, #6
  MOV R6, #8
  MOV R7, #9
  CMP R0, R1
  ITTEE EQ           ;if R0==R1, Then-Then-Else-Else
  ADDEQ R3, R4, R5   ;equal add
  ASREQ R3, R3, #1   ;equal shift right
  ADDNE R3, R6, R7   ;no equal add
  ASRNE R3, R3, #1   ;no equal shift right
  CMP R3, #8
  bne _ITT_Arithmetic_test_fail

  LDR  R1, =InstCheckPOST_struct     /*COUNT*/
  LDRB R0, [R1]                      /*LDRB*/
  ADD  R0,R0,#0x01
  STR  R0, [R1]

_ITT_Arithmetic_test_fail
;/* Pop the stack back */
  pop {r0-r12,r14}
;/* Branch back */
  bx  lr
;/**************************************/
;/* END of the _ITT_Arithmetic test     */
;/**************************************/


  END   ;֪ͨ���������ѴﵽԴ�ļ���ĩβ
  