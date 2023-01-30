#! /bin/bash

helper="list of options:
    -t   
    -p  
    -w  
    -h  
    -m  
    -f  
    -F  
    -G  
    -S  
    -A  
    -O  
    -Q  
    "

function main(){
  # Transform long options to short ones
  for arg in "$@"; do
    shift
    case "$arg" in
      '--help') set -- "$@" '-0';;
      '--tab') set -- "$@" '-7';;
      '--abr') set -- "$@" '-8';;
      '--avl') set -- "$@" '-9';;
      *) set -- "$@" "$arg";;
    esac
  done
  check_input $@
  check_C_application_config
  filter_rows__data_based_on_option
  filter_columns_and_create_file_based_on_variable
  launch_application
  rm $temp_input_file

}

function check_input(){
  # TODO: add option d of dates
  options=':f:tpwhmFGSAOQd0123'
  while getopts $options option; do
    case "$option" in
      t) temperature_flag='True';;
      p) pressure_flag='True';;
      w) wind_flag='True';;
      h) height_flag='True';;
      m) moister_flag='True';;
      f) file_flag='True'; input_file=$OPTARG;; #TODO check if file exists
      F) echo "${option}";;
      G) echo "${option}";;
      S) echo "${option}";;
      A) echo "${option}";;
      O) echo "${option}";;
      Q) echo "${option}";;
      0) echo $helper; exit 1;;
      7) echo "${option}";;
      8) echo "${option}";;
      9) echo "${option}";;
      :) printf "missing argument for -%s\n" "$OPTARG" >&2; echo -e "$helper" >&2; exit 1;;
     \?) printf "illegal option: -%s\n" "$OPTARG" >&2; echo -e "$helper" >&2; exit 1;;
    esac
  done

  if [ -z "$file_flag" ]; then 
    echo $file_flag
    echo "Missing mandatory argument file"
    echo -e $helper
    exit 1
  fi

  if [ -z "$temperature_flag" ] && [ -z "$pressure_flag" ] && [ -z "$wind_flag" ] && [ -z "$height_flag" ] && [ -z "$moister_flag" ]; then 
    echo "No arguments to analyze"
    echo $helper
    exit 1
  fi

}
# gcc -o application main.c

function check_C_application_config(){
  if [ ! -f "application" ];then
    if [ -f "main.c" ];then
      gcc -o application main.c
    else
      echo "No main.c found in folder. Compilation is not possible. Put the main.c in the folder"
      exit 1
    fi
  fi
}

function filter_rows__data_based_on_option(){
  # make a filtred copy
  temp_input_file="temp_input_file.csv"
  cp $input_file $temp_input_file
}


function filter_columns_and_create_file_based_on_variable(){
  # create a temp copy of the input_file
  if [ -n "$moister_flag" ]; then 
    moister_input_file="moister_data.csv"
    awk 'BEGIN{FS=OFS=";"}; {print $1,$6}' $temp_input_file > $moister_input_file
    echo "file filtred created successfully"
  fi
}

function launch_application(){
  ./application -f $moister_input_file -o moister_out_file.csv
}

function plot_results(){
  echo ''
}


main $@