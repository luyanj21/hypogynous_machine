################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/Delay.c \
../code/Encoder.c \
../code/Flash.c \
../code/Gpio.c \
../code/Icar.c \
../code/Motor.c \
../code/Pid.c \
../code/Servo.c \
../code/Timer.c \
../code/Usb.c 

COMPILED_SRCS += \
./code/Delay.src \
./code/Encoder.src \
./code/Flash.src \
./code/Gpio.src \
./code/Icar.src \
./code/Motor.src \
./code/Pid.src \
./code/Servo.src \
./code/Timer.src \
./code/Usb.src 

C_DEPS += \
./code/Delay.d \
./code/Encoder.d \
./code/Flash.d \
./code/Gpio.d \
./code/Icar.d \
./code/Motor.d \
./code/Pid.d \
./code/Servo.d \
./code/Timer.d \
./code/Usb.d 

OBJS += \
./code/Delay.o \
./code/Encoder.o \
./code/Flash.o \
./code/Gpio.o \
./code/Icar.o \
./code/Motor.o \
./code/Pid.o \
./code/Servo.o \
./code/Timer.o \
./code/Usb.o 


# Each subdirectory must supply rules for building sources it contributes
code/%.src: ../code/%.c code/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 -D__CPU__=tc26xb "-fC:/Users/cyl/Downloads/seekfree_assistant-master/seekfree_assistant-master/All code/TC264_demo/seekfree_assistant_oscilloscope_demo/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/code\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo 'Finished building: $<'
	@echo ' '

code/%.o: ./code/%.src code/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-code

clean-code:
	-$(RM) ./code/Delay.d ./code/Delay.o ./code/Delay.src ./code/Encoder.d ./code/Encoder.o ./code/Encoder.src ./code/Flash.d ./code/Flash.o ./code/Flash.src ./code/Gpio.d ./code/Gpio.o ./code/Gpio.src ./code/Icar.d ./code/Icar.o ./code/Icar.src ./code/Motor.d ./code/Motor.o ./code/Motor.src ./code/Pid.d ./code/Pid.o ./code/Pid.src ./code/Servo.d ./code/Servo.o ./code/Servo.src ./code/Timer.d ./code/Timer.o ./code/Timer.src ./code/Usb.d ./code/Usb.o ./code/Usb.src

.PHONY: clean-code

