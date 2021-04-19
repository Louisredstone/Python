# patterns=["♠️","♥️","♣️","♦️"]
patterns=[0,1,2,3]
# 3♠️ 2♥️ 1♣️ 0♦️
# Spade? Heart ? Diamond
points=[]
for i in range(12):
    points.append(i)
# 0:A 1:2 2:3 ... 9:10 10:J 11:Q 12:K

class Card:
    def __init__(self) -> None:
        pass

    def __str__(self) -> str:
        pass

class NormalPoker(Card):
    patternDict={
        0:"♦️",
        1:"♣️",
        2:"♥️",
        3:"♠️"
    }
    patterns=patternDict.keys()
    pointDict={
        0:"A",
        1:"2",2:"3",3:"4",4:"5",5:"6",6:"7",7:"8",8:"9",9:"10",
        10:"J",11:"Q",12:"K"
    }
    points=pointDict.keys()

    def __init__(self,pattern,point) -> None:
        super().__init__()
        assert pattern in patterns
        self.pattern=pattern
        assert point in points
        self.point=point

    def __str__(self) -> str:
        result=""
        result+=self.patternDict[self.pattern]
        result+=" "
        result+=self.pointDict[self.point]
        return result

class Joker(Card):
    def __init__(self,isBig=True) -> None:
        super().__init__()
        self.isBig=isBig
    
    def __str__(self) -> str:
        result="Big"if self.isBig else "Small"
        result+=" Joker"
        return result

class PokerSet():
    def __init__(self) -> None:
        self.cards=[]
        for patt in patterns:
            for point in points:
                self.cards.append(NormalPoker(patt,point))
        self.cards.append(Joker(True))
        self.cards.append(Joker(False))

    def __str__(self) -> str:
        result=""
        for card in self.cards:
            result+=card.__str__()+"\n"
        return result

    def shuffle(self):
        pass

pokerset=PokerSet()
print(pokerset)