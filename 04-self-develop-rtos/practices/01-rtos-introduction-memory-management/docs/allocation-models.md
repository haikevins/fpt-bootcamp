# Allocation Models

## Static allocation

Capacity rõ ràng, không external fragmentation, phù hợp production kernel
`static-first`.

## Stack allocation

Nhanh, lifetime theo scope, nhưng phải kiểm soát stack overflow.

## General-purpose heap

Linh hoạt nhưng có fragmentation, runtime failure và variable execution time.

## Fixed-size pool

Allocation thường O(1), capacity xác định, thích hợp cho Event/Message objects.
