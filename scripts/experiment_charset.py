import config
import experiment

# initialize experiment:
# - single-class classification problem using a synthetic task
# - the model should predict the digit of a synthetic image
cfg = config.config()
exp = experiment.experiment(
        cfg.task_synth_charset(ctype = "digit", color = "rgb", irows = 16, icols = 16, count = 10000),
        cfg.expdir + "/charset",
        trials = 10)

# loss functions
exp.add_loss("classnll", cfg.loss("classnll"))

# enhancers
exp.add_enhancer("default", cfg.enhancer("default"))

# trainers
batch = 32
epochs = 100
patience = 32
epsilon = 1e-6

exp.add_trainer("batch_gd", cfg.batch_trainer("gd", epochs=epochs, patience=patience, epsilon=epsilon))
exp.add_trainer("batch_cgd", cfg.batch_trainer("cgd", epochs=epochs, patience=patience, epsilon=epsilon))
exp.add_trainer("batch_lbfgs", cfg.batch_trainer("lbfgs", epochs=epochs, patience=patience, epsilon=epsilon))

exp.add_trainer("stoch_ag", cfg.batch_trainer("ag", epochs=epochs, patience=patience, epsilon=epsilon, batch=batch))
exp.add_trainer("stoch_agfr", cfg.batch_trainer("agfr", epochs=epochs, patience=patience, epsilon=epsilon, batch=batch))
exp.add_trainer("stoch_aggr", cfg.batch_trainer("aggr", epochs=epochs, patience=patience, epsilon=epsilon, batch=batch))

exp.add_trainer("stoch_sg", cfg.batch_trainer("sg", epochs=epochs, patience=patience, epsilon=epsilon, batch=batch))
exp.add_trainer("stoch_sgm", cfg.batch_trainer("sgm", epochs=epochs, patience=patience, epsilon=epsilon, batch=batch))
exp.add_trainer("stoch_ngd", cfg.batch_trainer("ngd", epochs=epochs, patience=patience, epsilon=epsilon, batch=batch))
exp.add_trainer("stoch_asgd", cfg.batch_trainer("asgd", epochs=epochs, patience=patience, epsilon=epsilon, batch=batch))
exp.add_trainer("stoch_svrg", cfg.batch_trainer("svrg", epochs=epochs, patience=patience, epsilon=epsilon, batch=batch))
exp.add_trainer("stoch_rmsprop", cfg.batch_trainer("rmsprop", epochs=epochs, patience=patience, epsilon=epsilon, batch=batch))

exp.add_trainer("stoch_adam", cfg.batch_trainer("adam", epochs=epochs, patience=patience, epsilon=epsilon, batch=batch))
exp.add_trainer("stoch_adagrad", cfg.batch_trainer("adagrad", epochs=epochs, patience=patience, epsilon=epsilon, batch=batch))
exp.add_trainer("stoch_adadelta", cfg.batch_trainer("adadelta", epochs=epochs, patience=patience, epsilon=epsilon, batch=batch))

# models
mlp0 = []
mlp1 = mlp0 + [128,1,1]
mlp2 = mlp1 + [256,1,1]
mlp3 = mlp2 + [512,1,1]
mlp4 = mlp3 + [1024,1,1]

cnn1 = [32,5,5,1,1,1]
cnn2 = cnn1 + [64,3,3,1,1,1]
cnn3 = cnn2 + [96,3,3,1,1,1]
cnn4 = cnn3 + [128,3,3,1,1,1]
cnn5 = cnn4 + [128,3,3,1,1,1]

exp.add_model("mlp0", cfg.mlp(mlp0, imaps=3, irows=16, icols=16, omaps=10, orows=1, ocols=1))
exp.add_model("mlp1", cfg.mlp(mlp1, imaps=3, irows=16, icols=16, omaps=10, orows=1, ocols=1))
exp.add_model("mlp2", cfg.mlp(mlp2, imaps=3, irows=16, icols=16, omaps=10, orows=1, ocols=1))
exp.add_model("mlp3", cfg.mlp(mlp3, imaps=3, irows=16, icols=16, omaps=10, orows=1, ocols=1))
exp.add_model("mlp4", cfg.mlp(mlp4, imaps=3, irows=16, icols=16, omaps=10, orows=1, ocols=1))

exp.add_model("cnn1", cfg.cnn(cnn1, mlp0, imaps=3, irows=16, icols=16, omaps=10, orows=1, ocols=1))
exp.add_model("cnn2", cfg.cnn(cnn2, mlp0, imaps=3, irows=16, icols=16, omaps=10, orows=1, ocols=1))
exp.add_model("cnn3", cfg.cnn(cnn3, mlp0, imaps=3, irows=16, icols=16, omaps=10, orows=1, ocols=1))
exp.add_model("cnn4", cfg.cnn(cnn4, mlp0, imaps=3, irows=16, icols=16, omaps=10, orows=1, ocols=1))

# train all configurations
exp.run_all()

# compare configurations
exp.summarize_by_models(".*")
exp.summarize_by_trainers("stoch*")
exp.summarize_by_trainers("batch*")
exp.summarize_by_trainers(".*")
