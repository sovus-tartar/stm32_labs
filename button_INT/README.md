# Lab XX

This repo contains my (successful) attempt to enable EXTI on STM32.

### Steps to enable

- Enable port thtough AHBENR
- Set alternative mode
- Set push-pull(be careful using open drain)
- Pull up/down set
- Enable clocking on SYSCFG and reset it (APB2ENR, APB2RSTR)
- Enable pins using EXTICR_X
- Enable raising/falling trigger (RTSR, FTSR)
- Enable NVIC using bit 5, 6 or 7 (see docs) in ISER
- Handler can differentiante activities from different pins
- IRQ Handler must be placed in vector table as stated in docs.
