#! /bin/bash

helper=
helper="list of options:\n
    -t  This option provides the results for the temperature. It will generate a file called: temperature_output_file.csv and a graph. It can be used in three modes.\n
    -p  This option provides us with the results for the atmospheric pressure. It will generate a file called: pressure_output_file.csv and a graph. It can be used in three modes.\n
    -w  This option outputs the average orientation and the average wind speed for each station and sorts them by ascending order of the identifier of the station. It will generate a file called: wind_output_file.csv and a graph.\n
    -h  This option will outputs the heights of each station and sorts them in descending order. It will generate a file called: height_output_file.csv and a graph.\n
    -m  This option will gives us the maximum humidity for each station and sorts them in descending order. It will generate a file called: moisture_output_file.csv and a graph.\n
    -f  This option gives the input files\n
    -r  This option sorts the files in descending order\n
    -F  This option will filter out all the stations that are not in Metropolitan France\n
    -G  This option will filter out all the stations that are not in French Guiana\n
    -S  This option will filter out all the stations that are not in Saint-Pierre-and-Miquelon\n
    -A  This option will filter out all the stations that are not in the West Indies\n
    -O  This option will filter out all the stations that are not in the Indian Ocean\n
    -Q  This option will filter out all the stations that are not in Antarctica\n
    -1  This option will provide results sorted by station\n
    -2  This option will provide results sorted by time stamp\n
    -3  This option will provide results sorted by time stamp then by station\n
    --avl set algo for sort to balanced tree\n
    --abr set algo for sort binary tree\n
    --tab set algo for sort array (linear data structure)\n
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
  generate_output_folders
  input_file_copy="input_copy.csv"
  cp $input_file $input_file_copy
  check_C_application_config
  rename_cols
  transform_coordinates_to_longitude_and_latitude
  geography_filter
  date_filter
  filter_columns_and_create_file_based_on_variable
  launch_application
  plot_all_option
  rm $input_file_copy

}

function check_input(){
  # TODO: add option d of dates
  sort_algo='avl' # default value
  options=':f:d:tpwhmrFGSAOQ0789123'
  while getopts $options option; do
    case "$option" in
      t) temperature_flag='True';;
      p) pressure_flag='True';;
      w) wind_flag='True';;
      h) height_flag='True';;
      m) moister_flag='True';;
      f) file_flag='True'; input_file=$OPTARG;; #TODO check if file exists
      r) reversed_flag='True';;
      F) france_flag='True';;
      G) guyane_flag='True';;
      S) canada_flag='True';;
      A) antilles_flag='True';;
      O) ocean_flag='True';;
      Q) antartic_flag='True';;
      d) date_flag='True';  
         args=("$OPTARG");
         until [[ $(eval "echo \${$OPTIND}") =~ ^-.* ]] || [ -z $(eval "echo \${$OPTIND}") ]; 
         do 
          args+=($(eval "echo \${$OPTIND}")) 
          OPTIND=$((OPTIND + 1)); 
         done ;
         min_date=${args[0]};
         max_date=${args[1]};
         pattern="^[0-9]{4}-[0-9]{2}-[0-9]{2}$"
         # Check if the variable matches the pattern of date required # check not null
         if [[ ! $min_date =~ $pattern ]] || [[ ! $max_date =~ $pattern ]] ; then
          echo -e "The dates are not in the right format\n"
          echo -e "$helper"; 
          exit 1;
         fi;;
      1) mode='1';;
      2) mode='2';;
      3) mode='3';;
      0) echo -e "$helper"; exit 1;;
      7) sort_algo='tab';;
      8) sort_algo='abr';;
      9) sort_algo='avl';;
      :) printf "missing argument for -%s\n" "$OPTARG" >&2; echo -e "$helper" >&2; exit 1;;
     \?) printf "illegal option: -%s\n" "$OPTARG" >&2; echo -e "$helper" >&2; exit 1;;
    esac
  done
  
  if [ -z "$file_flag" ]; then 
    echo $file_flag
    echo -e "Missing mandatory argument file\n"
    echo -e "$helper"
    exit 1
  fi

  if [ -z "$temperature_flag" ] && [ -z "$pressure_flag" ] && [ ! -z "$mode" ]; then 
    echo -e "You have to provide temperature or pressure option to define a mode. Read the manual below\n"
    echo -e "$helper"
    exit 1
  fi

  if [ -z "$mode" ]; then 
    mode='1' # default value
  fi

  if [ -z "$temperature_flag" ] && [ -z "$pressure_flag" ] && [ -z "$wind_flag" ] && [ -z "$height_flag" ] && [ -z "$moister_flag" ]; then 
    echo -e "No arguments to analyze\n"
    echo -e "$helper"
    exit 1
  fi

}

function generate_output_folders(){
  data_generated_folder="data_generation"
  moister_folder="${data_generated_folder}/moister"
  height_folder="${data_generated_folder}/height"
  wind_folder="${data_generated_folder}/wind"
  temperature_folder="${data_generated_folder}/temperature"
  pressure_folder="${data_generated_folder}/pressure"
  mkdir -p $data_generated_folder
  mkdir -p $moister_folder
  mkdir -p $height_folder
  mkdir -p $wind_folder
  mkdir -p $temperature_folder
  mkdir -p $pressure_folder
      
}

function check_C_application_config(){
  if [ ! -f "application" ];then
    if [ -d "src" ];then
      gcc -o application src/*.c
    else
      echo -e "No src folder found. Compilation is not possible.\n"
      exit 1
    fi
  fi
}

function date_filter(){
  if [ -n "$date_flag" ]; then
    echo "filter date"
    min_date="${min_date}T00:00:00+01:00"
    max_date="${max_date}T00:00:00+01:00"
    awk -v min_date="$min_date" -v max_date="$max_date" 'BEGIN{FS=OFS=";"};{if(NR==1)print $0; else if($2 < max_date && $2 > min_date ) print $0;}' $input_file_copy > tmp && mv tmp $input_file_copy
  fi
}

function geography_filter(){
  # filter files based on geography flag
  unicity_flag=0

  if [ ! -z $france_flag ]; then
    # values for test
    # longitude_max=49
    # longitude_min=47
    # latitude_max=-4
    # latitude_min=-10
    #real values
    longitude_max=9.7
    longitude_min=-5.5
    latitude_max=51.2
    latitude_min=41.2
    unicity_flag=$(($unicity_flag + 1))

  fi

  if [ ! -z $guyane_flag ]; then
    longitude_max=-51.4
    longitude_min=-67.5
    latitude_max=6.2
    latitude_min=1.8
    unicity_flag=$(($unicity_flag + 1))
  fi

  if [ ! -z $canada_flag ]; then
    longitude_max=-51
    longitude_min=-61.5
    latitude_max=51
    latitude_min=45.6
    unicity_flag=$(($unicity_flag + 1))
  fi

  if [ ! -z $antilles_flag ]; then
    longitude_max=-57
    longitude_min=-67.5
    latitude_max=18.8
    latitude_min=11.4
    unicity_flag=$(($unicity_flag + 1))
  fi


  if [ ! -z $ocean_flag ]; then
    longitude_max=99.6
    longitude_min=26.5
    latitude_max=-5.5
    latitude_min=-51.3
    unicity_flag=$(($unicity_flag + 1))
  fi


  if [ ! -z $antartic_flag ]; then
    longitude_max=178.3
    longitude_min=95.5
    latitude_max=-54.7
    latitude_min=-77.7
    unicity_flag=$(($unicity_flag + 1))
  fi

  if [ $unicity_flag -gt 1 ]; then
    echo -e "Please use only once the filter on geography\n"
    echo -e "$helper"
    exit 1
  elif [ $unicity_flag -eq 1 ]; then 
    awk -v longitude_max="$longitude_max" -v longitude_min="$longitude_min" -v latitude_max="$latitude_max" -v latitude_min="$latitude_min" 'BEGIN{FS=OFS=";"}; {if(NR==1)print $0; else if($16-longitude_max<0 && $16-longitude_min>0 && $17-latitude_max<0 && $17-latitude_min>0) print $0;}' $input_file_copy > tmp && mv tmp $input_file_copy
  fi
 
}

function filter_columns_and_create_file_based_on_variable(){
  # create a temp copy of the input_file
  if [ -n "$moister_flag" ]; then 
    moister_input_file="${moister_folder}/moister_data.csv"
    moister_output_file="${moister_folder}/moister_output_file.csv" 
    awk 'BEGIN{FS=OFS=";"}; {print $1,$6,$16,$17}' $input_file_copy > $moister_input_file
    awk -F ";" '{if($0!~/^;.*|.*;$|.*;;.*/) print}' $moister_input_file > tmp && mv tmp $moister_input_file
    echo "Moister file filtred created successfully"
  fi

  if [ -n "$height_flag" ]; then 
    height_input_file="${height_folder}/height_data.csv"
    height_output_file="${height_folder}/height_output_data.csv"
    awk 'BEGIN{FS=OFS=";"}; {print $1,$14,$16,$17}' $input_file_copy > $height_input_file
    awk -F ";" '{if($0!~/^;.*|.*;$|.*;;.*/) print}' $height_input_file > tmp && mv tmp $height_input_file
    echo "Height file filtred created successfully"
  fi

  if [ -n "$wind_flag" ]; then 
    wind_input_file="${wind_folder}/wind_data.csv"
    wind_output_file="${wind_folder}/wind_output_data.csv"
    awk 'BEGIN{FS=OFS=";"}; {print $1,$4,$5,$16,$17}' $input_file_copy > $wind_input_file
    awk -F ";" '{if($0!~/^;.*|.*;$|.*;;.*/) print}' $wind_input_file > tmp && mv tmp $wind_input_file
    echo "Wind file filtred created successfully"
  fi

  if [ -n "$temperature_flag" ]; then 
    temperature_input_file="${temperature_folder}/temperature_${mode}_data.csv"
    temperature_output_file="${temperature_folder}/temperature_${mode}_output_data.csv"
    awk 'BEGIN{FS=OFS=";"}; {print $1,$2,$11,$12,$13,$16,$17}' $input_file_copy > $temperature_input_file
    awk -F ";" '{if($0!~/^;.*|.*;$|.*;;.*/) print}' $temperature_input_file > tmp && mv tmp $temperature_input_file
    echo "Temperature file filtred created successfully"
  fi

  if [ -n "$pressure_flag" ]; then 
    pressure_input_file="${pressure_folder}/pressure_${mode}_data.csv"
    pressure_output_file="${pressure_folder}/pressure_${mode}_output_data.csv"
    awk 'BEGIN{FS=OFS=";"}; {print $1,$2,$7,$7,$7,$16,$17}' $input_file_copy > $pressure_input_file
    pressure_header="ID_station;Date;Pression_station;Pression_min;Pression_max;Longitude;Latitude"
    sed -i "1s/.*/${pressure_header}/;" $pressure_input_file
    awk -F ";" '{if($0!~/^;.*|.*;$|.*;;.*/) print}' $pressure_input_file > tmp && mv tmp $pressure_input_file
    echo "Pressure file filtred created successfully"
  fi
}

function launch_application(){

  if [ -z $reversed_flag ]; then

    if [ -n "$moister_flag" ]; then 
      echo "moister"
      ./application -f $moister_input_file -o $moister_output_file $sort_algo 
    fi

    if [ -n "$height_flag" ]; then 
      echo "height"
      ./application -f $height_input_file -o $height_output_file $sort_algo 
    fi

    if [ -n "$wind_flag" ]; then 
      echo "wind"
      ./application -f $wind_input_file -o $wind_output_file $sort_algo 
    fi

    if [ -n "$temperature_flag" ]; then 
      echo "temp"
      ./application -f $temperature_input_file -o $temperature_output_file $sort_algo 
    fi

    if [ -n "$pressure_flag" ]; then 
      echo "press"
      ./application -f $pressure_input_file -o $pressure_output_file $sort_algo 
    fi

  else
    if [ -n "$moister_flag" ]; then 
      ./application -r -f $moister_input_file -o $moister_output_file $sort_algo 
    fi

    if [ -n "$height_flag" ]; then 
      ./application -r -f $height_input_file -o $height_output_file $sort_algo 
    fi

    if [ -n "$wind_flag" ]; then 
      ./application -r -f $wind_input_file -o $wind_output_file $sort_algo 
    fi

    if [ -n "$temperature_flag" ]; then 
      ./application -r -f $temperature_input_file -o $temperature_output_file $sort_algo 
    fi

    if [ -n "$pressure_flag" ]; then 
      ./application -r -f $pressure_input_file -o $pressure_output_file $sort_algo 
    fi

  fi


}

function plot_results(){
  echo ''
}

function rename_cols(){
  # rename all columns in any case on the file copy
  # TO DO: add all wanted renaming
  sed -i '1s/ID OMM station/ID_station/;' $input_file_copy
  sed -i '1s/Pression au niveau mer/Pression_niveau_mer/;' $input_file_copy
  sed -i '1s/Direction du vent moyen 10 mn/Direction_vent/;' $input_file_copy
  sed -i '1s/Vitesse du vent moyen 10 mn/Vitesse_vent/;' $input_file_copy
  sed -i '1s/Pression station/Pression_station/;' $input_file_copy
  sed -i '1s/Variation de pression en 24 heures/Variation_pression/;' $input_file_copy
  sed -i '1s/Température (°C)/Temperature/;' $input_file_copy
  sed -i '1s/Température minimale sur 24 heures (°C)/Temperature_min/;' $input_file_copy
  sed -i '1s/Température maximale sur 24 heures (°C)/Temperature_max/;' $input_file_copy
}

function transform_coordinates_to_longitude_and_latitude(){
  # Add at the end of columns of longitude and latitude
  cat $input_file_copy | tr -d '\r' > input_tmp.csv
  mv input_tmp.csv $input_file_copy
  cut -f 10 -d ';' $input_file_copy | sed 1d > tmp.csv
  sed -i '1s/^/Longitude,Latitude\n/' tmp.csv
  tr ',' ';'< tmp.csv > tmp_processed.csv
  rm tmp.csv
  paste -d";" $input_file_copy tmp_processed.csv > output.csv
  rm tmp_processed.csv
  mv output.csv $input_file_copy
}


function postprocess_data(){
  echo "postprocess_data"
}

function plot_all_option(){

  if [ -n "$moister_flag" ]; then 
    moister_plot_file="${moister_folder}/moister_plot_file.csv"
    awk 'BEGIN{FS=OFS=";"}; {print $3,$4,$2}' $moister_output_file > $moister_plot_file
    sed -i '1d' $moister_plot_file # delete firt row
    sed -i G $moister_plot_file # split data to blocks
    gnuplot -e  "set datafile separator ';';
    set terminal pngcairo size 700,500 enhanced font 'Verdana,10';
    set output 'data_generation/moister/output_moister.png';
    set pm3d map;
    set palette defined ( 0 0.0 0.5 0.5, 1 1.0 0.0 0.0);
    set cblabel 'colour gradient';
    set view map;
    splot 'data_generation/moister/moister_plot_file.csv' using 1:2:3 with lines;"
    echo "Moister plot done"
  fi

  if [ -n "$height_flag" ]; then 
    height_plot_file="${height_folder}/height_plot_file.csv"
    awk 'BEGIN{FS=OFS=";"}; {print $3,$4,$2}' $height_output_file > $height_plot_file
    sed -i '1d' $height_plot_file # delete firt row
    sed -i G $height_plot_file # split data to blocks
    gnuplot -e  "set datafile separator ';';
    set terminal pngcairo size 700,500 enhanced font 'Verdana,10';
    set output 'data_generation/height/output_height.png';
    set pm3d;
    set palette defined ( 0 0.0 0.5 0.5, 1 1.0 0.0 0.0);
    set cblabel 'colour gradient';
    set view map;
    splot 'data_generation/height/height_plot_file.csv' using 1:2:3 with lines;"
    echo "Height plot done"
  fi

  if [ -n "$wind_flag" ]; then 
    wind_plot_file="${wind_folder}/wind_plot_file.csv"
    awk 'BEGIN{FS=OFS=";"}; {print $1,$2}' $wind_output_file > $wind_plot_file
    sed -i '1d' $wind_plot_file # delete firt row
    gnuplot -e  "set datafile separator ';';
    set terminal pngcairo size 700,500 enhanced font 'Verdana,10';
    set output 'data_generation/wind/output_wind.png';
    plot 'data_generation/wind/wind_plot_file.csv' using 1:2 with lines;";
    echo "Wind plot done"
  fi

  if [ -n "$temperature_flag" ]; then 
    temperature_plot_file="${temperature_folder}/temperature_plot_file.csv"
    awk 'BEGIN{FS=OFS=";"}; {print $2,$3}' $temperature_output_file > $temperature_plot_file
    sed -i '1d' $temperature_plot_file # delete firt row
    gnuplot -e  "set datafile separator ';';
    set terminal pngcairo size 700,500 enhanced font 'Verdana,10';
    set output 'data_generation/temperature/output_temperature.png';
    set xdata time;
    set timefmt '%Y-%m-%dT%H:%M:%S+01:00';
    plot 'data_generation/temperature/temperature_plot_file.csv' using 1:2 with lines;";
    echo "Temperature plot done"
  fi

  if [ -n "$pressure_flag" ]; then 
    pressure_plot_file="${pressure_folder}/pressure_plot_file.csv"
    awk 'BEGIN{FS=OFS=";"}; {print $2,$3}' $pressure_output_file > $pressure_plot_file
    sed -i '1d' $pressure_plot_file # delete firt row
    gnuplot -e  "set datafile separator ';';
    set terminal pngcairo size 700,500 enhanced font 'Verdana,10';
    set output 'data_generation/pressure/output_pressure.png';
    set xdata time;
    set timefmt '%Y-%m-%dT%H:%M:%S+01:00';
    plot 'data_generation/pressure/pressure_plot_file.csv' using 1:2 with lines;";
    echo "Pressure plot done"
  fi


}

main $@