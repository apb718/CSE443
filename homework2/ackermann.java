// Copyright (C) 2016 raodm@miamiOH.edu

// A simple implementation of an Ackermann method.

public class ackermann {

    public static void main(String[] args) {
	int num = Integer.parseInt(args[0]);
	System.out.println("Ack(3," + num + "): " + Ack(3, num));
    }

    public static int Ack(int m, int n) {
        if (m > 0) {
            if (n > 0) {
                return Ack(m - 1, Ack(m, n - 1));
            } else {
                return Ack(m - 1, 1);
            }
        } else {
            return n + 1;
        }
    }
}

