# 🔵 Type-2 Hypervisor — VMware Workstation

This folder contains the **Type-2 / hosted hypervisor** portion of the experiment.

## VM Configuration

| Parameter | Value |
|---|---|
| OS | Ubuntu |
| CPU | 2 vCPU |
| RAM | 2 GB |
| Disk | 20 GB |
| Network | NAT |
| Benchmark | Sysbench CPU |

## Procedure

1. Open VMware Workstation.
2. Create a new VM using the Ubuntu ISO.
3. Select Linux → Ubuntu 64-bit.
4. Allocate 20 GB disk, 2 vCPU and 2048 MB RAM.
5. Configure the network adapter as NAT.
6. Install Ubuntu.
7. Verify the VM configuration.
8. Install Sysbench.
9. Run the CPU benchmark.
10. Shut down the VM.

## Commands

```bash
sudo apt update
sudo apt install sysbench -y
sysbench --version
sysbench cpu --cpu-max-prime=20000 run
```

```bash
hostnamectl
lscpu
free -h
df -h
top
sudo poweroff
```

## 📸 Evidence

### 04 — CPU configuration
![Type-2 CPU](../assets/04_type2_lscpu_1.jpg)

### 05 — Additional CPU details
![Type-2 additional CPU](../assets/05_type2_lscpu_2.jpg)

### 06 — Memory configuration
![Type-2 memory](../assets/06_type2_memory.jpg)

### 07 — Sysbench benchmark
![Type-2 Sysbench](../assets/07_type2_sysbench.jpg)

### 08 — Detailed Sysbench output
![Type-2 detailed Sysbench](../assets/08_type2_sysbench_details.jpg)

## Result

- **Events/sec:** 1277.56
- **Total events:** 12778
- **Average latency:** 0.78 ms
