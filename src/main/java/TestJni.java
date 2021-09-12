import com.zenvv.capi.*;

public class TestJni {
    public static void main(String[] args) {
        ADecoder dec = new ADecoder(ADecoder.PCMA_CODEC);
        byte[] data = new byte[100];
        byte[] ret = dec.Decode(data);
        dec.Close();
        System.out.println("hello world:" + ret.length);
    }
}
