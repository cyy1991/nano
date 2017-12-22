import os
import json

class config:
        def __init__(self, build_folder = "../build-release"):
                # useful paths
                homedir = os.path.expanduser('~')
                self.expdir = os.path.join(homedir, "experiments", "results")
                self.dbdir = os.path.join(homedir, "experiments", "databases")

                crtpath = os.path.dirname(os.path.realpath(__file__))
                self.app_info = os.path.join(crtpath, build_folder, "apps", "info")
                self.app_train = os.path.join(crtpath, build_folder, "apps", "train")
                self.app_stats = os.path.join(crtpath, build_folder, "apps", "stats")
                self.app_builder = os.path.join(crtpath, build_folder, "apps", "builder")
                self.app_tabulate = os.path.join(crtpath, build_folder, "apps", "tabulate")

        # available losses
        def losses(self):
                return ["cauchy",
                        "square",
                        "classnll",
                        "s-logistic",
                        "m-logistic",
                        "s-exponential",
                        "m-exponential"]

        # available enhancers
        def enhancers(self):
                return ["default",
                        "noise",
                        "warp",
                        "noclass"]

        # available activations
        def activations(self):
                return ["act-unit",
                        "act-sin",      # [-1, +1]
                        "act-tanh",     # [-1, +1]
                        "act-splus",    # [ 0,  1]
                        "act-snorm",    # [-1, +1]
                        "act-ssign",    # [-1, +1]
                        "act-sigm",     # [ 0,  1]
                        "act-pwave"]    # [-1, +1]

        # helper utilities to create models
        def model(model_type, conv3d_param, affine_param, imaps, irows, icols, omaps, orows, ocols, act_type):
                return  "--{} --act-type {} ".format(model_type, act_type) +
                        "--conv3d-param {} ".format(','.join(conv3d_param)) +
                        "--affine-param {} ".format(','.join(affine_param)) +
                        "--imaps {} --irows {} --icols {} ".format(imaps, irows, icols) +
                        "--omaps {} --orows {} --ocols {} ".format(omaps, orows, ocols)

        def mlp(affine_param, imaps, irows, icols, omaps, orows, ocols, act_type="act-snorm"):
                return model("mlp", affine_param, [], imaps, irows, icols, omaps, orows, ocols, act_type)

        def cnn(conv3d_param, affine_param, imaps, irows, icols, omaps, orows, ocols, act_type="act-snorm"):
                return model("cnn", conv3d_param, affine_param, imaps, irows, icols, omaps, orows, ocols, act_type)

        # available stochastic solvers
        def stoch_solvers(self):
                return ["ag", "agfr", "aggr",
                        "sg", "sgm", "ngd", "svrg", "asgd",
                        "adagrad", "adadelta", "adam", "rmsprop"]

        # available batch solvers
        def batch_solvers(self):
                return ["gd", "cgd", "lbfgs"]

        # configure stochastic trainers
        def trainer_stoch(self, solver, epochs = 100, patience = 32, epsilon = 1e-6, batch = 32):
                assert(solver in self.stoch_solvers())
                return json.dumps({
                        "solver": solver, "epochs": epochs, "patience": patience, "epsilon": epsilon, "batch": batch})

        # configure batch trainers
        def trainer_batch(self, solver, epochs = 100, patience = 32, epsilon = 1e-6):
                assert(solver in self.batch_solvers())
                return json.dumps({
                        "solver": solver, "epochs": epochs, "patience": patience, "epsilon": epsilon})

        # configure losses
        def loss(self, loss):
                assert(loss in self.losses())
                return json.dumps({"loss": loss})

        # configure enhancers
        def enhancer(self, enhancer):
                assert(enhancer in self.enhancers())
                return json.dumps({"enhancer": enhancer})

        # configure tasks
        def task(self, name):
                return json.dumps({"task": name, "dir": os.path.join(self.dbdir, name)})

        def task_mnist(self):
                return self.task("mnist")

        def task_cifar10(self):
                return self.task("cifar10")

        def task_iris(self):
                return self.task("iris")

        def task_wine(self):
                return self.task("wine")

        def task_synth_charset(self, ctype = "digit", color = "rgb", irows = 16, icols = 16, count = 10000):
                return json.dumps({
                        "task": "synth-charset", "type": ctype, "color": color, "irows": irows, "icols": icols, "count": count})

        def task_synth_nparity(self, n = 32, count = 10000):
                return json.dumps({"task": "synth-nparity", "n": n, "count": count})
