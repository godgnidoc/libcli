import { Application, Element } from "./application";
import { Feature } from "./feature";
export declare const comp: {
    /** 正在执行自动补全 */
    completeing: boolean;
    /** 执行自动补全时，光标尚未离开最后一个单词 */
    editing: string;
    /** 自动补全的响应结果 */
    response: string[];
};
export declare const cli: {
    app: Application;
    options: any;
    path: string[];
    element: Element;
    features: Set<Feature>;
};
