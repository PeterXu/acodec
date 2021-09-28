import com.zenvv.capi.*;

public class TestJni {
    public static void main(String[] args) {
        byte[] data = new byte[960];
        AEncoder enc = new AEncoder(AEncoder.OPUS_CODEC);
        byte[] ret = enc.Encode(data);
        int err = enc.GetLastError();
        enc.Close();
        int len = (ret != null) ? ret.length : 0;
        System.out.println("encode:" + len + ", err:" + err);

        {
            ADecoder dec1 = new ADecoder(ADecoder.OPUS_CODEC);
            byte[] ret1 = dec1.Decode(ret);
            int err1 = dec1.GetLastError();
            dec1.Close();
            int len1 = (ret1 != null) ? ret1.length : 0;
            System.out.println("decode1:" + len1 + ", err:" + err1);
        }

        {
            ADecoder dec2 = new ADecoder(ADecoder.OPUS_CODEC);
            short[] ret2 = dec2.DecodeEx(ret);
            int err2 = dec2.GetLastError();
            dec2.Close();
            int len2 = (ret2 != null) ? ret2.length : 0;
            System.out.println("decode2:" + len2 + ", err:" + err2);
        }
    }
}
