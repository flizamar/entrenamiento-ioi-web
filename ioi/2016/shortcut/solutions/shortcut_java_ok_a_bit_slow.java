import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.io.Writer;
import java.util.Arrays;
import java.util.InputMismatchException;
import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * Built using CHelper plug-in
 * Actual solution is at the top
 *
 * @author Egor Kulikov (egor@egork.net)
 */
class shortcut {
    int[] interesting;
    int n;
    long c;
    long[] pos;
    int[] d;
    int maxPos;

    public void solve(int testNumber, shortcut_java_ok_a_bit_slow.InputReader in, shortcut_java_ok_a_bit_slow.OutputWriter out) {
        int n = in.readInt();
        int c = in.readInt();
        int[] l = shortcut_java_ok_a_bit_slow.IOUtils.readIntArray(in, n - 1);
        int[] d = shortcut_java_ok_a_bit_slow.IOUtils.readIntArray(in, n);
        out.printLine("14e047d7a2907b9034950b074822b302");
        out.printLine(find_shortcut(n, l, d, c));
    }

    boolean can(long limit) {
        long maxSum = Long.MAX_VALUE;
        long minSum = Long.MIN_VALUE;
        long maxDif = Long.MAX_VALUE;
        long minDif = Long.MIN_VALUE;
        int at = 0;
        for (int i : interesting) {
            if (interesting[at] < i) {
                at++;
            }
            while (at + 1 < interesting.length) {
                long dist = d[i] + d[interesting[at + 1]] + pos[interesting[at + 1]] - pos[i];
                if (dist > limit) {
                    break;
                }
                at++;
            }
            if (at + 1 == interesting.length) {
                break;
            }
            long dist = d[i] + d[interesting[at + 1]] + c;
            if (dist > limit) {
                return false;
            }
            long delta = limit - dist;
            long sum = pos[i] + pos[interesting[at + 1]];
            maxSum = Math.min(maxSum, sum + delta);
            minSum = Math.max(minSum, sum - delta);
            long dif = pos[i] - pos[interesting[at + 1]];
            maxDif = Math.min(maxDif, dif + delta);
            minDif = Math.max(minDif, dif - delta);
            dist = d[i] + d[interesting[interesting.length - 1]] + c;
            if (dist > limit) {
                return false;
            }
            delta = limit - dist;
            sum = pos[i] + pos[interesting[interesting.length - 1]];
            maxSum = Math.min(maxSum, sum + delta);
            minSum = Math.max(minSum, sum - delta);
            dif = pos[i] - pos[interesting[interesting.length - 1]];
            maxDif = Math.min(maxDif, dif + delta);
            minDif = Math.max(minDif, dif - delta);
        }
        for (int i = 0; i < interesting[0]; i++) {
            if (d[i] + d[interesting[0]] + pos[interesting[0]] - pos[i] <= limit) {
                continue;
            }
            long dist = d[i] + d[interesting[0]] + c;
            if (dist > limit) {
                return false;
            }
            long delta = limit - dist;
            long sum = pos[i] + pos[interesting[0]];
            maxSum = Math.min(maxSum, sum + delta);
            minSum = Math.max(minSum, sum - delta);
            long dif = pos[i] - pos[interesting[0]];
            maxDif = Math.min(maxDif, dif + delta);
            minDif = Math.max(minDif, dif - delta);
        }
        for (int i = 0; i < maxPos; i++) {
            if (d[i] + d[maxPos] + pos[maxPos] - pos[i] <= limit) {
                continue;
            }
            long dist = d[i] + d[maxPos] + c;
            if (dist > limit) {
                return false;
            }
            long delta = limit - dist;
            long sum = pos[i] + pos[maxPos];
            maxSum = Math.min(maxSum, sum + delta);
            minSum = Math.max(minSum, sum - delta);
            long dif = pos[i] - pos[maxPos];
            maxDif = Math.min(maxDif, dif + delta);
            minDif = Math.max(minDif, dif - delta);
        }
        for (int i = maxPos + 1; i < n; i++) {
            if (d[i] + d[maxPos] - pos[maxPos] + pos[i] <= limit) {
                continue;
            }
            long dist = d[i] + d[maxPos] + c;
            if (dist > limit) {
                return false;
            }
            long delta = limit - dist;
            long sum = pos[i] + pos[maxPos];
            maxSum = Math.min(maxSum, sum + delta);
            minSum = Math.max(minSum, sum - delta);
            long dif = -pos[i] + pos[maxPos];
            maxDif = Math.min(maxDif, dif + delta);
            minDif = Math.max(minDif, dif - delta);
        }
        for (int i = interesting[interesting.length - 1] + 1; i < n; i++) {
            if (d[i] + d[interesting[interesting.length - 1]] - pos[interesting[interesting.length - 1]] + pos[i] <=
                    limit) {
                continue;
            }
            long dist = d[i] + d[interesting[interesting.length - 1]] + c;
            if (dist > limit) {
                return false;
            }
            long delta = limit - dist;
            long sum = pos[i] + pos[interesting[interesting.length - 1]];
            maxSum = Math.min(maxSum, sum + delta);
            minSum = Math.max(minSum, sum - delta);
            long dif = -pos[i] + pos[interesting[interesting.length - 1]];
            maxDif = Math.min(maxDif, dif + delta);
            minDif = Math.max(minDif, dif - delta);
        }
        if (maxDif < minDif || maxSum < minSum) {
            return false;
        }
        at = n - 1;
        for (int i = 0; i < n; i++) {
            while (at + 1 < n && maxSum >= pos[i] + pos[at + 1] && minDif <= pos[i] - pos[at + 1]) {
                at++;
            }
            while (at > i && (maxSum < pos[i] + pos[at] || minDif > pos[i] - pos[at])) {
                at--;
            }
            if (at <= i) {
                continue;
            }
            if (minSum <= pos[i] + pos[at] && maxDif >= pos[i] - pos[at]) {
                return true;
            }
        }
        return false;
    }

    long find_shortcut(int n, int[] l, int[] d, int c) {
        this.n = n;
        this.d = d;
        this.c = c;
        pos = new long[n];
        for (int i = 0; i < n - 1; i++) {
            pos[i + 1] = pos[i] + l[i];
        }
        boolean[] viable = new boolean[n];
        Arrays.fill(viable, true);
        long max = -1;
        long current = 0;
        for (int i = 0; i < n; i++) {
            if (max >= current + d[i]) {
                viable[i] = false;
            } else {
                max = current + d[i];
            }
            if (i < n - 1) {
                current += l[i];
            }
        }
        max = -1;
        current = 0;
        for (int i = n - 1; i >= 0; i--) {
            if (max >= current + d[i]) {
                viable[i] = false;
            } else {
                max = current + d[i];
            }
            if (i > 0) {
                current += l[i - 1];
            }
        }
        shortcut_java_ok_a_bit_slow.IntList list = new shortcut_java_ok_a_bit_slow.IntArrayList();
        for (int i = 0; i < n; i++) {
            if (viable[i]) {
                list.add(i);
            }
        }
        maxPos = shortcut_java_ok_a_bit_slow.ArrayUtils.maxPosition(d);
        interesting = list.toArray();
        long left = shortcut_java_ok_a_bit_slow.ArrayUtils.maxElement(d);
        long right = shortcut_java_ok_a_bit_slow.ArrayUtils.sumArray(l) + 2 * shortcut_java_ok_a_bit_slow.ArrayUtils.maxElement(d);
        while (left < right) {
            long middle = (left + right) >> 1;
            if (can(middle)) {
                right = middle;
            } else {
                left = middle + 1;
            }
        }
        return left;
    }

}


public class shortcut_java_ok_a_bit_slow {
    public static void main(String[] args) {
        InputStream inputStream = System.in;
        OutputStream outputStream = System.out;
        InputReader in = new InputReader(inputStream);
        OutputWriter out = new OutputWriter(outputStream);
        shortcut solver = new shortcut();
        solver.solve(1, in, out);
        out.close();
    }

    static class ArrayUtils {
        public static long sumArray(int[] array) {
            return new IntArray(array).sum();
        }

        public static int maxElement(int[] array) {
            return new IntArray(array).max();
        }

        public static int maxPosition(int[] array) {
            return new IntArray(array).maxIndex();
        }

    }

    static class IntArray extends IntAbstractStream implements IntList {
        private int[] data;

        public IntArray(int[] arr) {
            data = arr;
        }

        public int size() {
            return data.length;
        }

        public int get(int at) {
            return data[at];
        }

        public void addAt(int index, int value) {
            throw new UnsupportedOperationException();
        }

        public void removeAt(int index) {
            throw new UnsupportedOperationException();
        }

    }

    static interface IntReversableCollection extends IntCollection {
    }

    static class InputReader {
        private InputStream stream;
        private byte[] buf = new byte[1024];
        private int curChar;
        private int numChars;
        private InputReader.SpaceCharFilter filter;

        public InputReader(InputStream stream) {
            this.stream = stream;
        }

        public int read() {
            if (numChars == -1) {
                throw new InputMismatchException();
            }
            if (curChar >= numChars) {
                curChar = 0;
                try {
                    numChars = stream.read(buf);
                } catch (IOException e) {
                    throw new InputMismatchException();
                }
                if (numChars <= 0) {
                    return -1;
                }
            }
            return buf[curChar++];
        }

        public int readInt() {
            int c = read();
            while (isSpaceChar(c)) {
                c = read();
            }
            int sgn = 1;
            if (c == '-') {
                sgn = -1;
                c = read();
            }
            int res = 0;
            do {
                if (c < '0' || c > '9') {
                    throw new InputMismatchException();
                }
                res *= 10;
                res += c - '0';
                c = read();
            } while (!isSpaceChar(c));
            return res * sgn;
        }

        public boolean isSpaceChar(int c) {
            if (filter != null) {
                return filter.isSpaceChar(c);
            }
            return isWhitespace(c);
        }

        public static boolean isWhitespace(int c) {
            return c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == -1;
        }

        public interface SpaceCharFilter {
            public boolean isSpaceChar(int ch);

        }

    }

    static class OutputWriter {
        private final PrintWriter writer;

        public OutputWriter(OutputStream outputStream) {
            writer = new PrintWriter(new BufferedWriter(new OutputStreamWriter(outputStream)));
        }

        public OutputWriter(Writer writer) {
            this.writer = new PrintWriter(writer);
        }

        public void print(Object... objects) {
            for (int i = 0; i < objects.length; i++) {
                if (i != 0) {
                    writer.print(' ');
                }
                writer.print(objects[i]);
            }
        }

        public void printLine(Object... objects) {
            print(objects);
            writer.println();
        }

        public void close() {
            writer.close();
        }

        public void printLine(long i) {
            writer.println(i);
        }

    }

    static class IOUtils {
        public static int[] readIntArray(InputReader in, int size) {
            int[] array = new int[size];
            for (int i = 0; i < size; i++) {
                array[i] = in.readInt();
            }
            return array;
        }

    }

    static interface IntStream extends Iterable<Integer>, Comparable<IntStream> {
        public IntIterator intIterator();

        default public Iterator<Integer> iterator() {
            return new Iterator<Integer>() {
                private IntIterator it = intIterator();

                public boolean hasNext() {
                    return it.isValid();
                }

                public Integer next() {
                    int result = it.value();
                    it.advance();
                    return result;
                }
            };
        }

        default public int compareTo(IntStream c) {
            IntIterator it = intIterator();
            IntIterator jt = c.intIterator();
            while (it.isValid() && jt.isValid()) {
                int i = it.value();
                int j = jt.value();
                if (i < j) {
                    return -1;
                } else if (i > j) {
                    return 1;
                }
                it.advance();
                jt.advance();
            }
            if (it.isValid()) {
                return 1;
            }
            if (jt.isValid()) {
                return -1;
            }
            return 0;
        }

        default public int max() {
            int result = Integer.MIN_VALUE;
            for (IntIterator it = intIterator(); it.isValid(); it.advance()) {
                int current = it.value();
                if (current > result) {
                    result = current;
                }
            }
            return result;
        }

        default public long sum() {
            long result = 0;
            for (IntIterator it = intIterator(); it.isValid(); it.advance()) {
                result += it.value();
            }
            return result;
        }

    }

    static class IntArrayList extends IntAbstractStream implements IntList {
        private int size;
        private int[] data;

        public IntArrayList() {
            this(3);
        }

        public IntArrayList(int capacity) {
            data = new int[capacity];
        }

        public IntArrayList(IntCollection c) {
            this(c.size());
            addAll(c);
        }

        public IntArrayList(IntStream c) {
            this();
            if (c instanceof IntCollection) {
                ensureCapacity(((IntCollection) c).size());
            }
            addAll(c);
        }

        public IntArrayList(IntArrayList c) {
            size = c.size();
            data = c.data.clone();
        }

        public IntArrayList(int[] arr) {
            size = arr.length;
            data = arr.clone();
        }

        public int size() {
            return size;
        }

        public int get(int at) {
            if (at >= size) {
                throw new IndexOutOfBoundsException("at = " + at + ", size = " + size);
            }
            return data[at];
        }

        private void ensureCapacity(int capacity) {
            if (data.length >= capacity) {
                return;
            }
            capacity = Math.max(2 * data.length, capacity);
            data = Arrays.copyOf(data, capacity);
        }

        public void addAt(int index, int value) {
            ensureCapacity(size + 1);
            if (index > size || index < 0) {
                throw new IndexOutOfBoundsException("at = " + index + ", size = " + size);
            }
            if (index != size) {
                System.arraycopy(data, index, data, index + 1, size - index);
            }
            data[index] = value;
            size++;
        }

        public void removeAt(int index) {
            if (index >= size || index < 0) {
                throw new IndexOutOfBoundsException("at = " + index + ", size = " + size);
            }
            if (index != size - 1) {
                System.arraycopy(data, index + 1, data, index, size - index - 1);
            }
            size--;
        }

    }

    static interface IntIterator {
        public int value() throws NoSuchElementException;

        public boolean advance();

        public boolean isValid();

    }

    static abstract class IntAbstractStream implements IntStream {

        public String toString() {
            StringBuilder builder = new StringBuilder();
            boolean first = true;
            for (IntIterator it = intIterator(); it.isValid(); it.advance()) {
                if (first) {
                    first = false;
                } else {
                    builder.append(' ');
                }
                builder.append(it.value());
            }
            return builder.toString();
        }


        public boolean equals(Object o) {
            if (!(o instanceof IntStream)) {
                return false;
            }
            IntStream c = (IntStream) o;
            IntIterator it = intIterator();
            IntIterator jt = c.intIterator();
            while (it.isValid() && jt.isValid()) {
                if (it.value() != jt.value()) {
                    return false;
                }
                it.advance();
                jt.advance();
            }
            return !it.isValid() && !jt.isValid();
        }


        public int hashCode() {
            int result = 0;
            for (IntIterator it = intIterator(); it.isValid(); it.advance()) {
                result *= 31;
                result += it.value();
            }
            return result;
        }

    }

    static interface IntCollection extends IntStream {
        public int size();

        default public void add(int value) {
            throw new UnsupportedOperationException();
        }

        default public int[] toArray() {
            int size = size();
            int[] array = new int[size];
            int i = 0;
            for (IntIterator it = intIterator(); it.isValid(); it.advance()) {
                array[i++] = it.value();
            }
            return array;
        }

        default public IntCollection addAll(IntStream values) {
            for (IntIterator it = values.intIterator(); it.isValid(); it.advance()) {
                add(it.value());
            }
            return this;
        }

    }

    static interface IntList extends IntReversableCollection {
        public abstract int get(int index);

        public abstract void addAt(int index, int value);

        public abstract void removeAt(int index);

        default public IntIterator intIterator() {
            return new IntIterator() {
                private int at;
                private boolean removed;

                public int value() {
                    if (removed) {
                        throw new IllegalStateException();
                    }
                    return get(at);
                }

                public boolean advance() {
                    at++;
                    removed = false;
                    return isValid();
                }

                public boolean isValid() {
                    return !removed && at < size();
                }

                public void remove() {
                    removeAt(at);
                    at--;
                    removed = true;
                }
            };
        }


        default public void add(int value) {
            addAt(size(), value);
        }

        default public int maxIndex() {
            int result = Integer.MIN_VALUE;
            int size = size();
            int at = -1;
            for (int i = 0; i < size; i++) {
                int current = get(i);
                if (current > result) {
                    result = current;
                    at = i;
                }
            }
            return at;
        }

    }
}

