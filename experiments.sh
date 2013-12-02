#!/bin/bash

# paths
dir_results=/home/cosmin/experiments/results
dir_db=/home/cosmin/experiments/databases

exe_trainer=./build/ncv_trainer

batch_params="opt=lbfgs,eps=1e-4,iters=1024"
common_params="--loss classnll --threads 1"
conv_params="conv8x8:convs=16;snorm"

# task description = task model [model-params] trainer trainer-params trials output
tasks=(
	"mnist forward-network batch ${batch_params} 1 mnist-affine"	
	"mnist forward-network ${conv_params} batch ${batch_params} 1 mnist-hidden1"
	"mnist forward-network ${conv_params};${conv_params} batch ${batch_params} 1 mnist-hidden2"
	"mnist forward-network ${conv_params};${conv_params};${conv_params} batch ${batch_params} 1 mnist-hidden3"
	
	"cbcl-faces forward-network batch ${batch_params} 1 cbclfaces-affine"
	"cbcl-faces forward-network ${conv_params} batch ${batch_params} 1 cbclfaces-hidden1"
	"cbcl-faces forward-network ${conv_params};${conv_params} batch ${batch_params} 1 cbclfaces-hidden2"
	
	"cifar10 forward-network batch ${batch_params} 1 cifar10-affine"
	"cifar10 forward-network ${conv_params} batch ${batch_params} 1 cifar10-hidden1"
	"cifar10 forward-network ${conv_params};${conv_params} batch ${batch_params} 1 cifar10-hidden2"
	"cifar10 forward-network ${conv_params};${conv_params};${conv_params} batch ${batch_params} 1 cifar10-hidden3"
	
	# TODO: STL10
	)

# run tasks
for ((i=0; i<${#tasks[*]}; i++))
do
	task=${tasks[$i]}	
	arr=(${task})

	task_id=${arr[0]}
	model_id=${arr[1]}

	dir_exp=${dir_results}/${task_id}
	mkdir -p ${dir_exp}

	task_dir=${dir_db}/${task_id}

	if [[ ${#arr[*]} -eq 7 ]]
	then 
		model_params=${arr[2]}
		trainer_id=${arr[3]}
		trainer_params=${arr[4]}
		trials=${arr[5]}	
		output=${dir_exp}/${arr[6]}.model
	        log=${dir_exp}/${arr[6]}.log
	else
		model_params=
		trainer_id=${arr[2]}
                trainer_params=${arr[3]}
                trials=${arr[4]}        
                output=${dir_exp}/${arr[5]}.model
                log=${dir_exp}/${arr[5]}.log
	fi	

	config=""
	config=${config}" --task ${task_id} --task-dir ${task_dir}"
	config=${config}" --trials ${trials} --output ${output} ${common_params}"                
	if [[ -z ${model_params} ]]
	then
		config=${config}" --model ${model_id}"
	else
                config=${config}" --model ${model_id} --model-params ${model_params}"
	fi
	config=${config}" --trainer ${trainer_id} --trainer-params ${trainer_params}"
	
	echo "running <${task_id}> ..."
	echo -e "\twith model   <${model_id}><${model_params}>"
        echo -e "\twith trainer <${trainer_id}><${trainer_params}>"
	echo -e "\twith param   <${common_params}>"

	time ${exe_trainer} ${config} > ${log}

	echo -e "\tlog saved to <${log}>"
	echo
done

