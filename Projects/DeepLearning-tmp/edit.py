import torch
import torchvision as tv
import torchvision.transforms as transforms
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
import os

class GoogleNetV1(nn.Module):
    def __init__(self,classes=1000):
        super(GoogleNetV1,self).__init__()
        class Inception(nn.Module):
            def __init__(self,inputShape):
                pass
            def forward(self,x):
                pass
    def forward(self,x):
        pass

class InceptionA(nn.Module):
    def __init__(self,in_channels):
        super(InceptionA,self).__init__()
        self.branch1x1 = nn.Conv2d(in_channels,16,kernel_size=1)

        self.branch5x5_1 = nn.Conv2d(in_channels,16,kernel_size=1)
        self.branch5x5_2 = nn.Conv2d(16,24,kernel_size=5,padding=2)

        self.branch3x3_1 = nn.Conv2d(in_channels,16,kernel_size=1)
        self.branch3x3_2 = nn.Conv2d(16,24,kernel_size=3,padding=1)
        self.branch3x3_3 = nn.Conv2d(24,24,kernel_size=3,padding=1)

        self.branch_pool = nn.Conv2d(in_channels,24,kernel_size=1)

    def forward(self,x):
        xbranch1x1 = self.branch1x1(x)
        xbranch5x5 = self.branch5x5_2(
                    self.branch5x5_1(x)
                    )
        xbranch3x3 = self.branch3x3_3(
                    self.branch3x3_2(
                    self.branch3x3_1(x)
                    ))
        xbranch_pool = self.branch_pool(
            F.avg_pool2d(x,kernel_size=3,stride=1,padding=1)
        )
        outputs = [ xbranch1x1,
                    xbranch5x5,
                    xbranch3x3,
                    xbranch_pool]
        return torch.cat(outputs,dim=1)