import com.zenvv.capi.*;

public class TestJni {
    public static void main(String[] args) {
        ADecoder dec = new ADecoder(ADecoder.PCMA_CODEC);
        dec.Close();
        System.out.println("hello world");
    }
}
