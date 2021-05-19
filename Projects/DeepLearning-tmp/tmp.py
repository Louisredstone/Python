class Cl1:
  class Cl2:
   def __init__(self):
    print("init of Cl2")
  def __init__(self):
   print("creating a Cl2")
   cl2=self.Cl2()
   print("init of Cl1 done")

cl1=Cl1()