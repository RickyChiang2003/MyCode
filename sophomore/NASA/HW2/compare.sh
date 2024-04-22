#!/usr/bin/env bash
export LC_ALL=C

error_input=0
no_opt=1
is_a=0
is_h=0
is_l=0
is_n=0
EXP_n=""
is_r=0
file_arr=()

function find_r(){
    local PATH_CO="$2"
    dir_arr=()
    if [ $is_a -eq 1 ]; then
	dir_arr=(`ls -a "$1""$PATH_CO"`)
    else
	dir_arr=(`ls "$1""$PATH_CO"`)
    fi
    for FILE in ${dir_arr[@]}; do
	if [ $FILE == "." ] || [ $FILE == ".." ]; then
	    continue
	fi
	PATH_C="$PATH_CO""$FILE"
	nexPATH="$1""$PATH_C"
	if [ -h $nexPATH ] && [ $is_l -eq 0 ]; then
	    continue
	fi

	if [ -f $nexPATH ] || [ -h $nexPATH ]; then
	    found=0
	    for i in ${file_arr[@]}; do
		if [ $i == $PATH_C ]; then
		    found=1
		    break
		fi
	    done
	    if [ $found -eq 0 ]; then
		file_arr+=($PATH_C)
	    fi
	fi
	if [ -d $nexPATH ] && [ ! -h $nexPATH ]; then
	    PATH_C="$PATH_C""/"
	    find_r $1 $PATH_C
	fi
    done
}


#----------------------------------------------------------
#                          main
# ---------------------------------------------------------

#----------------------------------------------------------
# 1. check input format
#----------------------------------------------------------
while getopts ":ahln:r" opt
do
    no_opt=0
    case $opt in
	a)
	    is_a=1
	    ;;
	h)
	    is_h=1
	    ;;
	l)
	    is_l=1
	    ;;
	n)
	    is_n=1
	    if [[ -z $OPTARG ]]; then
		error_input=1
	    fi
	    EXP_n=$OPTARG
	    ;;
	r)
	    is_r=1
	    ;;
	?) # rule 1-4
	    error_input=1
	    ;;
    esac
done   
# 1.1 meet the constraints of -r input
if ([[ $is_a -eq 1 ]] || [[ $is_n -eq 1 ]]) && [[ is_r -eq 0 ]]; then
    error_input=1
elif [[ $OPTIND -ne $(($#-1)) ]]; then
    error_input=1
fi

if [[ $error_input -eq 0 ]] && [[ $is_h -eq 0 ]]; then
    # 1.1.1 let $1, $2 to be the two target files
    shift $(($OPTIND - 1))
    if ([ $is_r -eq 1 ] && [ -d $1 ] && [ ! -d $2 ]) || ([ $is_r -eq 1 ] && [ ! -d $1 ] && [ -d $2 ]); then
	error_input=1

#----------------------------------------------------------
# 2. process no -r cases
#----------------------------------------------------------
    elif ([ $is_r -eq 0 ] && [ ! -h $1 ] && [ ! -h $2 ] && [ -f $1 ] && [ -f $2 ]) ||\
	 ([ $is_l -eq 1 ] && [ -h $1 ] && [ -h $2 ] && [ $is_r -eq 0 ]) ||\
	 ([ $is_l -eq 1 ] && [ -f $1 ] && [ -h $2 ] && [ $is_r -eq 0 ]) ||\
	 ([ $is_l -eq 1 ] && [ -h $1 ] && [ -f $2 ] && [ $is_r -eq 0 ]); then
	# 2.1. process the "changed 100%" cases
	if ([ -h $1 ] || [ -h $2 ]) && ([ ! -h $1 ] || [ ! -h $2 ]); then
	    echo "changed 100%"
	    exit
	fi
	if ([ -d $1 ] || [ -d $2 ]) && ([ ! -d $1 ] || [ ! -d $2 ]); then
	    echo "changed 100%"
	    exit
	fi
	if [ -d $1 ] && [ -d $2 ] && [ $1 != $2 ]; then
	    echo "changed 100%"
	    exit
	fi
	if [[ -h $1 ]] && [[ -h $2 ]] && ([[ $1 == $2 ]] || [[ $(readlink $1) == $(readlink $2) ]]); then
	    exit
	fi
	DIFF=$(diff -d $1 $2)
	DIFF2=(`diff -d $1 $2`)
	if [[ $DIFF2 == "二元碼檔" ]] || [[ $DIFF2 == "Binary" ]] || [[ $DIFF2 == "binary" ]]; then
	    echo "changed 100%"
	    exit
	fi
	if [[ -h $1 ]] && ([[ -n $DIFF ]] || [[ $1 != $2 ]]); then
	    echo "changed 100%"
	    exit
	fi
	
	# 2.2. count a,b,c and x
	is_readable=1
	t1=0
	t3=0
	for i in ${DIFF2[@]}; do
	    if [ $is_readable -eq 0 ]; then
		is_readable=1
		continue
	    fi
	    if [ $i == ">" ]; then
		t1=$(($t1+1))
		is_readable=0
	    elif [ $i == "<" ]; then
		t3=$(($t3+1))
		is_readable=0
	    fi
	done
	arg_a=$t3
	arg_b=$t1
	arg_c=(`grep -c "" $1`)
	arg_c=$(($arg_c - $arg_a))
	tmp=$(($arg_a > $arg_b ? $arg_a : $arg_b))
	arg_x=$(( $(($tmp*100)) / $(($tmp+$arg_c)) ))

	if [[ -n $DIFF ]]; then
	    echo "changed ${arg_x}%"
	fi

#----------------------------------------------------------
# 3. process -r cases
#----------------------------------------------------------
    elif [ $is_r -eq 1 ]; then
	PATH_A=$1"/"
	PATH_B=$2"/"

	# 3.1. put accepted files below PATH_A into file_arr[]
	dir_arr=()
	if [ $is_a -eq 1 ]; then
	    dir_arr=(`ls -a "$PATH_A"`)
	else
	    dir_arr=(`ls "$PATH_A"`)
	fi
	for FILE in ${dir_arr[@]}; do
	    if [ $FILE == "." ] || [ $FILE == ".." ]; then
		continue
	    fi
	    PATH_C="$FILE"
	    nexPATH="$PATH_A""$PATH_C"
	    if [ -h $nexPATH ] && [ $is_l -eq 0 ]; then
		continue
	    fi

	    if [ -f $nexPATH ] || [ -h $nexPATH ]; then
		file_arr+=($PATH_C)
	    fi
	    if [ -d $nexPATH ] && [ ! -h $nexPATH ]; then
		PATH_C="$PATH_C""/"
		find_r $PATH_A $PATH_C
	    fi
	done

	# 3.2. put accepted (and also unique) files below PATH_B into file_arr[]
	if [ $is_a -eq 1 ]; then
	    dir_arr=(`ls -a "$PATH_B"`)
	else
	    dir_arr=(`ls "$PATH_B"`)
	fi
	for FILE in ${dir_arr[@]}; do
	    if [ $FILE == "." ] || [ $FILE == ".." ]; then
		continue
	    fi
	    PATH_C="$FILE"
	    nexPATH="$PATH_B""$PATH_C"
	    if [ -h $nexPATH ] && [ $is_l -eq 0 ]; then
		continue
	    fi

	    if [ -f $nexPATH ] || [ -h $nexPATH ]; then
		found=0
		for i in ${file_arr[@]}; do
		    if [ $i == $PATH_C ]; then
			found=1
			break
		    fi
		done
		if [ $found -eq 0 ]; then
		    file_arr+=($PATH_C)
		fi
	    fi
	    if [ -d $nexPATH ] && [ ! -h $nexPATH ]; then
		PATH_C="$PATH_C""/"
		find_r $PATH_B $PATH_C
	    fi
	done
	
	# 3.3. sort file_arr[] into file_sorted[]
	file_sorted=()
	readarray -td '' file_sorted < <(printf '%s\0' "${file_arr[@]}" | sort -z -n)

	# 3.4. check constraint and existence on each file, and then output
	for FILE in ${file_sorted[@]}; do
	    
	    # 3.4.1. check <EXP> if there is -n
	    if [ $is_n -eq 1 ]; then
		if [[ ! $FILE =~ $EXP_n ]]; then
		    continue
		fi
	    fi
	    
	    # 3.4.2. check if FILE_A and FILE_B exist under constraints
	    FILE_A="$PATH_A""$FILE"
	    FILE_B="$PATH_B""$FILE"
	    AOK=0
	    BOK=0
	    if ([ ! -h $FILE_A ] && [ -f $FILE_A ]) || ([ $is_l -eq 1 ] && [ -h $FILE_A ]); then
		AOK=1
	    fi
	    if ([ ! -h $FILE_B ] && [ -f $FILE_B ]) || ([ $is_l -eq 1 ] && [ -h $FILE_B ]); then
		BOK=1
	    fi

	    # 3.4.3. check existence and constraints
	    if [ $AOK -eq 1 ] && [ $BOK -eq 0 ]; then
		echo "delete $FILE"
	    elif [ $AOK -eq 0 ] && [ $BOK -eq 1 ]; then
		echo "create $FILE"
	    elif [ $AOK -eq 1 ] && [ $BOK -eq 1 ]; then
		if [ $is_l -eq 1 ] && [ -h $FILE_A ] && [ -h $FILE_B ]; then
		    read1=$(readlink $FILE_A)
		    read2=$(readlink $FILE_B)
		    if [[ $FILE_A == $FILE_B ]] || [[ $(readlink $FILE_A) == $(readlink $FILE_B) ]]; then
			continue
		    else
			echo "$FILE: changed 100%"
			continue
		    fi
		fi
		DIFF="$(diff $FILE_A $FILE_B)"
		if [[ -z $DIFF ]]; then
		    continue
		fi
		# 3.4.3.1. output by execute compare.sh again
		change_s=""
		if [ $is_l -eq 1 ]; then
		    change_s=$(./compare.sh -l $FILE_A $FILE_B)
		else
		    change_s=$(./compare.sh $FILE_A $FILE_B)
		fi
		output="$FILE: $change_s"
		echo "$output"
	    fi
	done	
    else
	error_input=1
    fi
fi


# output usage and options if there is -h or error_input
if [[ $error_input -eq 1 ]] || [[ $is_h -eq 1 ]]; then
    echo "usage: ./compare.sh [OPTION] <PATH A> <PATH B>"
    echo "options:"
    echo "-a: compare hidden files instead of ignoring them"
    echo "-h: output information about compare.sh"
    echo "-l: treat symlinks as files instead of ignoring them"
    echo "-n <EXP>: compare only files whose paths follow the REGEX <EXP>"
    echo "-r: compare directories recursively"
fi
#


