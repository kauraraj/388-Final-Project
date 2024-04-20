# 388-Final-Project
Final Project for 388 Raj and Jamie

if (distance > 200){
    greed led
    x = safeDistance, no breaking
}
else if (100 < distance <= 200) {
    yellow led (red + green)
    x = closeDistance, break lightly
} else if (60 < distance <= 100) {
    red led
    x = very close, breka hard
} else {
    x = too close, stop condition
    flashing red led (flashing period of 100 ms)
}