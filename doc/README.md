# About
This is the project to transmit and receive data throw RTMP streams. For example, over Rutube, VK video Live and WebRTC video channels in the future.
# IP package structure
```mermaid
flowchart TB
subgraph Layer1 ["Layer 1: Colored macro-blocks"]
    direction TB
    subgraph Layer2 ["Layer 2: Corrected data"]
        direction TB
        subgraph Layer3 ["Layer 3: Compressed data"]
            direction TB
            subgraph Layer4 ["Layer 4: Obfuscated data"]
                direction TB
                subgraph Layer5 ["Layer 5: payload"]
                    
                end
            end
        end
    end
end

style Layer1 fill:#1b4332,stroke:#081c15,stroke-width:2px,stroke-dasharray: 5 5,color:#ffffff,rx:30,ry:30
style Layer2 fill:#40916c,stroke:#1b4332,stroke-width:2px,color:#ffffff,rx:25,ry:25
style Layer3 fill:#95d5b2,stroke:#2d6a4f,stroke-width:2px,color:#081c15,rx:20,ry:20
style Layer4 fill:#ffb703,stroke:#fb8500,stroke-width:2px,color:#023047,rx:15,ry:15
style Layer5 fill:#fb8500,stroke:#d00000,stroke-width:2px,color:#ffffff,rx:10,ry:10
```
# Project structure
![](map.svg)