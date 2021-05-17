import random as rd
import numpy as np
import matplotlib.pyplot as plt

class Human:
    def __init__(self) -> None:
        self.farm=np.random.normal(5,1)#种地的能力
        self.food=np.random.normal(5,1)#吃饭的需求
        #这个人种比较普通，他种地的能力和吃饭的需求看上去基本相当
        self.alive=True
    def simulate(self):
        if self.farm>=self.food:
            self.live()
        else:
            self.die()
    def live(self):
        self.alive=True
    def die(self):
        self.alive=False

class Society:
    def __init__(self,n) -> None:
        self.people=[]
        for i in range(n):
            self.people.append(Human())
    def simulate(self):
        # people=self.people
        # rd.shuffle(people)
        rd.shuffle(self.people)
        for human in self.people:
            human.simulate()
    def plot(self):
        # people=self.people
        # rd.shuffle(people)
        farm_stat=[]
        food_stat=[]
        for human in self.people:
            if human.alive:
                farm_stat.append(human.farm)
                food_stat.append(human.food)
        params=dict(histtype="stepfilled",alpha=0.3,bins=200)
        plt.hist(farm_stat,color="green",label="farm",**params)
        plt.hist(food_stat,color="orange",label="food",**params)
        plt.legend()
        # plt.show()


society=Society(10000)
plt.subplot(131)
plt.title("day 0")
society.plot()

society.simulate()
plt.subplot(132)
plt.title("day 1")
society.plot()

for i in range(9):
    society.simulate()
plt.subplot(133)
plt.title("day 10")
society.plot()



plt.show()
# input()