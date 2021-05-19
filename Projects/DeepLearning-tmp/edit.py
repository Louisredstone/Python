from collections import OrderedDict
import torch
from torch._C import InferredType, InterfaceType
from torch.nn.modules import flatten, padding
from torch.nn.modules.batchnorm import BatchNorm2d
from torch.nn.modules.instancenorm import InstanceNorm1d
from torch.nn.modules.pooling import MaxPool2d
import torchvision as tv
import torchvision.transforms as transforms
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
import os
import re

class AnyNet(nn.Module):
    def __init__(self,arch):
        super().__init__()
        arch="""# debug
        
        """
        arch=re.sub(r"[ \t]","",arch)
        arch=re.sub(r"\n+","\n",arch)
        arch=re.sub(r"(^\n)|(\n$)","",arch)
        arch=re.sub(r"#[^\n]*\n","",arch)#去掉注释

class YOLO(nn.Module):
# 《You Only Look Once: Unified, Real-Time Object Detection》
    def __init__(self):
        super().__init__()
        arch="""
        7x7x64/2
        maxpool 2x2/2
        3x3x192
        maxpool 2x2/2
        1x1x128
        3x3x256
        1x1x256
        3x3x512
        maxpool 2x2/2
        {1x1x256
        3x3x512}x4
        1x1x512
        3x3x1024
        maxpool 2x2/2
        {1x1x512
        3x3x1024}x2
        3x3x1024
        3x3x1024/2
        3x3x1024
        3x3x1024
        """
        arch=re.sub(r"[ \t]","",arch)
        arch=re.sub(r"\n+","\n",arch)
        arch=re.sub(r"(^\n)|(\n$)","",arch)
        arch=re.sub(r"#[^\n]*\n","",arch)#去掉注释
        for layerstr,n in re.findall(r"{([\d\w\n]*)}x(\d+)",arch):
            old_str="{"+layerstr+"}x"+n
            new_str="\n".join([layerstr]*int(n))
            arch=arch.replace(old_str,new_str,1)
        layers=[]
        in_channels=3
        for line in arch.split("\n"):
            if line.startswith("maxpool"):
                line=re.sub("^maxpool","",line)
                kr,kc,stride=re.search(r"(\d+)x(\d+)(/\d+)?",line).groups()
                kr=int(kr)
                kc=int(kc)
                stride=1 if stride==None else int(stride[1:])
                if kr!=kc:
                    raise "kr!=kc"
                layers.append(
                    nn.MaxPool2d(kernel_size=kr,stride=stride)
                )
            else:
                kr,kc,out_channels,stride=re.search(r"(\d+)x(\d+)x(\d+)(/\d+)?",line).groups()
                kr=int(kr)
                kc=int(kc)
                out_channels=int(out_channels)
                stride=1 if stride==None else int(stride[1:])
                if kr!=kc:
                    raise "kr!=kc"
                layers+=[
                    nn.Conv2d(in_channels=in_channels,out_channels=out_channels,kernel_size=kr,stride=stride,padding=int((kr-1)/2)),
                    nn.LeakyReLU(0.1)
                ]
                in_channels=out_channels
        layers+=[
            nn.Flatten(),
            nn.Linear(in_features=in_channels*7*7,out_features=4096),
            nn.Linear(in_features=4096,out_features=7*7*30)
        ]
        self.layers=nn.Sequential(*layers)
    def forward(self,x):
        return self.layers(x)

class ResNet50(nn.Module):
    class Bottleneck(nn.Module):
        def __init__(self,in_channels,out_channels,firstConvStride=2) -> None:
            super().__init__()
            och1,och2=out_channels
            self.layers=nn.Sequential(
                nn.Conv2d(in_channels=in_channels,out_channels=och1,kernel_size=1,stride=firstConvStride,padding=0),
                nn.BatchNorm2d(num_features=och1),
                nn.ReLU(),
                nn.Conv2d(in_channels=och1,out_channels=och1,kernel_size=3,stride=1,padding=1),
                nn.BatchNorm2d(num_features=och1),
                nn.ReLU(),
                nn.Conv2d(in_channels=och1,out_channels=och2,kernel_size=1,stride=1,padding=0),
                nn.BatchNorm2d(num_features=och2),
                nn.ReLU()
            )
            self.finalReLU=nn.ReLU()
        def forward(self,x):
            return self.finalReLU(
                self.layer(x)+x
            )

    def __init__(self) -> None:
        super().__init__()
        # input
        # (b x 3 x 224 x 224)
        self.layers=nn.Sequential(
            nn.Conv2d(in_channels=3,out_channels=64,kernel_size=7,stride=2,padding=3),
        # (b x 64 x 112 x 112)
            nn.MaxPool2d(kernel_size=3,stride=2,padding=1),
        # (b x 64 x 56 x 56)
            self.Bottleneck(in_channels=64,out_channels=(64,256),firstConvStride=1),
        # (b x 256 x 56 x 56)
            self.Bottleneck(in_channels=256,out_channels=(128,512)),
        # (b x 512 x 28 x 28)
            self.Bottleneck(in_channels=512,out_channels=(256,1024)),
        # (b x 1024 x 14 x 14)
        )