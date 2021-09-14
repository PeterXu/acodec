import com.zenvv.capi.*;

public class TestJni {
    public static void main(String[] args) {
        byte[] data = new byte[960];
        AEncoder enc = new AEncoder(AEncoder.OPUS_CODEC);
        byte[] ret1 = enc.Encode(data);
        enc.Close();
        System.out.println("hello world:" + ret1.length);

        ADecoder dec = new ADecoder(ADecoder.OPUS_CODEC);
        byte[] ret2 = dec.Decode(ret1);
        dec.Close();
        System.out.println("hello world:" + ret2.length);

    }
}
